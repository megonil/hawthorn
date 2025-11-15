#include <share/string.h>
#include <stdint.h>

int8_t str_eq(str* string, const str* other_str)
{
	if (string->length != other_str->length)
	{
		return STR_NEQ;
	}

	for (str_size i = 0; i < string->length; i++)
	{
		if (string->value[i] == other_str->value[i])
		{
			continue;
		}

		else
		{
			return STR_NEQ;
		}
	}

	return STR_EQ;
}
str make_str(cstr c_string)
{
	str str;
	str.length = strlen(c_string);
	str.value  = strndup(c_string, str.length);

	return str;
}
