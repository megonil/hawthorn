#ifndef haw_type_h
#define haw_type_h

#include <limits.h>
#include <share/hawthorn.h>

// types
typedef enum
{
	HAW_TNONE = -1,
	HAW_TVOID,
	HAW_TINT,
	HAW_TNUMBER,
	HAW_TOBJECT,
	HAW_TFN,
	HAW_TTHREAD,
} HawType;

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

#define HAW_INT_MAX INT_MAX
#define HAW_INT_MIN INT_MIN
#endif // !haw_type_h
