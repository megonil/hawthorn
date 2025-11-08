#ifndef haw_type_h
#define haw_type_h

#include <share/hawthorn.h>

// types
typedef enum
{
	HAW_TNONE = -1,
	HAW_TVOID,
	HAW_TBOOL,
	HAW_TNUMBER,
	HAW_TSTRING,
	HAW_TOBJECT,
	HAW_TFN,
	HAW_TTHREAD,
} HawTypes;

// some typedefs
typedef unsigned char hawu_byte;
typedef signed char	  haws_byte;
typedef int			  haw_int;
typedef float		  haw_number;

#endif // !haw_type_h
