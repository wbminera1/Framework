#include <stdio.h>
#include "Log_Assert.h"

void _Log_Assert(int level, const char*  _Message, const char*  _File, unsigned int _Line)
{
	char buffer[1024];
	snprintf(buffer, sizeof(buffer) - 1, " Assert failed '%s' %s:%d", _Message, _File, _Line);
	Log(level, buffer);
}


