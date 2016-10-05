#include <stdarg.h>
#include <stdio.h>

#include <Windows.h>

#include "Log.h"

static int LogLevel = LOG_DEBUG;

void SetLogLevel(int level)
{
	if (level < LOG_LAST) {
		LogLevel = level;
	}
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

	  //
	  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	  WORD levelColors[LOG_LAST] = { FOREGROUND_RED | FOREGROUND_INTENSITY, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, FOREGROUND_GREEN | FOREGROUND_INTENSITY, FOREGROUND_INTENSITY };
	  SetConsoleTextAttribute(hConsole, levelColors[level]);
	  puts(buffer);

	  strncat(buffer, "\n", sizeof(buffer) - 1);
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
