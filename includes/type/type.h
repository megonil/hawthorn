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
typedef double		  haw_number;
typedef unsigned int  haw_unsigned;

#define cast(T, v) ((T) v)
#define cast_hawint(V) cast(haw_int, (V))
#define cast_hawnumber(V) cast(haw_number, (V))
#define cast_hawubyte(V) cast(hawu_byte, (V))
#define cast_hawsbyte(V) cast(haws_byte, (V))

#endif // !haw_type_h
