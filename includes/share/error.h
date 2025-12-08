#ifndef megonil_error
#define megonil_error

#include "util.h"

#define error(MSG) throw_error(MODULE_NAME, (MSG))

#define error_with_line(MSG) throw_error_with_line(MODULE_NAME, (MSG), __LINE__)

#define error_at(MSG, AT) throw_error_with_line(MODULE_NAME, (MSG), (AT))

#define errorf(...) throw_errorf(MODULE_NAME, __VA_ARGS__)

noret throw_error_with_line(const char* module_name, const char* error_msg, int line);
noret throw_error(const char* module_name, const char* error_msg);
noret throw_errorf(const char* module_name, const char* msg, ...);

#define massert(cond, msg, ...)                                                                    \
	if (!(cond))                                                                                   \
	{                                                                                              \
		throw_errorf(MODULE_NAME, msg, __VA_ARGS__);                                               \
	}

#endif
