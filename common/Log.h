#ifndef LOG_H_
#define LOG_H_

#define LOG_ERR		0
#define LOG_WARNING	1
#define LOG_INFO	2
#define LOG_DEBUG	3
#define LOG_LAST	4

void Log(int level, const char* format, ...);
void SetLogLevel(int level);

#endif // LOG_H_
