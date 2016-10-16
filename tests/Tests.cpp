#include <windows.h>
#include <stdio.h>
#include "../common/Log_Assert.h"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

int Server_Test();
int Socket_Test();
int Thread_Test();

template <class R, class S>
class A
{
};

class B 
{
public:
	class Nested 
	{
	public:
		virtual void print() {
			printf(__FUNCTION__ "\n");
		}
	};
};

class C : public A<B::Nested, B::Nested>, public B
{

};

/*
int main(_In_ int _Argc, _In_reads_(_Argc) _Pre_z_ char ** _Argv, _In_z_ char ** _Env)
{
	//Log_Assert(LOG_ERR, _Argc > 5);
	//Thread_Test();
	Server_Test();
	B b;
	return 0;
}
*/
