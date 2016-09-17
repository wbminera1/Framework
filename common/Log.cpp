#include <stdarg.h>
#include <stdio.h>

#include <Windows.h>

#include "Log.h"

static int LogLevel = LOG_DEBUG;

void SetLogLevel(int level)
{
  LogLevel = level;
}

void Log(int level, const char* format, ...)
{
  if (level <= LogLevel)
  {
	  va_list args;
	  va_start(args, format);
	  char buffer[1024];
	  int loff = snprintf(buffer, sizeof(buffer) - 1, "<%d>", level);
	  vsnprintf(buffer + loff, sizeof(buffer) - loff - 1, format, args);
	  va_end(args);

	  puts(buffer);

	  OutputDebugStringA(buffer); 

	  static FILE* lf = NULL;
	  if (lf == nullptr)
	  {
		  lf = fopen("log.txt", "wt");
	  }
	  if (lf != nullptr)
	  {
		  fputs(buffer, lf);
		  fflush(lf);
	  }
  }
}
