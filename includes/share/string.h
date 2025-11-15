#ifndef megonil_string
#define megonil_string

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef int32_t				 str_size;
typedef char				 base_char;
typedef const char*			 cstr;
typedef char*				 cstr_mut;
typedef const unsigned char* ucstr;
typedef unsigned char*		 ucstr_mut;

typedef struct
{
	str_size   length;
	base_char* value;
} str;

#define this str* string
#define this_t str

str make_str(cstr c_string);

#define STR_EQ 1  // result value when strings are equal
#define STR_NEQ 0 // result value when string are not equal
#define STR_DL -1 // result value when string have different lengths

int8_t str_eq(this, const this_t* other_str);

#undef this
#endif // !megonil_string
