#ifndef LOG_ASSERT_H_
#define LOG_ASSERT_H_
#include "Log.h"

void _Log_Assert(
	int level,
	const char*  _Message,
	const char*  _File,
	unsigned int _Line
);

#define Log_Assert(level, expression) (void)(                                                       \
            (!!(expression)) ||                                                              \
            (_Log_Assert(level, (#expression), (__FILE__), (unsigned)(__LINE__)),0) \
        )


#endif // LOG_ASSERT_H_
