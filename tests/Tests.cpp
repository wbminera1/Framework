#include <windows.h>
#include "../common/Log_Assert.h"

int Server_Test();
int Socket_Test();
int Thread_Test();


int main(_In_ int _Argc, _In_reads_(_Argc) _Pre_z_ char ** _Argv, _In_z_ char ** _Env)
{
	Log_Assert(LOG_ERR, _Argc > 5);
	//Thread_Test();
	Server_Test();
	return 0;
}

