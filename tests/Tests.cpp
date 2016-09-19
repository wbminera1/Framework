#include <windows.h>

int Server_Test();
int Socket_Test();
int Thread_Test();


int main(_In_ int _Argc, _In_reads_(_Argc) _Pre_z_ char ** _Argv, _In_z_ char ** _Env)
{
	Server_Test();
	return 0;
}

