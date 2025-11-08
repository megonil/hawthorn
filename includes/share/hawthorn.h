#ifndef hawthorn_h
#define hawthorn_h

#define HAW_VERSION "0.0.1"
#define HAW_AUTHOR "megonilus"

// For later use
#include "hawconf.h"

// thread status
typedef enum
{
	THREAD_OK,
	THREAD_YIELD,
	THREAD_ERRRUN,
	THREAD_ERRSYNTAX,
	THREAD_ERRMEM,
	THREAD_ERRERR
} ThreadStatus;

#endif // !hawthorn_h
