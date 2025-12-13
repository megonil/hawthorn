#ifndef haw_value_h
#define haw_value_h

#include "share/string.h"

#include <stdio.h>
#include <type/type.h>

// values of all types are first-class values: we can store them
// in global variables, local variables and table fields, pass them as arguments to
// functions, return them from functions, etc.
// strings like userdata and gcobject will stored as a reference
typedef union
{
	// gcobject in bright future...
	// function...
	haw_int	   int_;	// integer
	haw_number number_; // float/double
	String*	   str_;	// string
} Value;

// tagged values like in lua
typedef struct
{
	Value	value_;
	HawType type;
} TValue;

#define t_isint(o) ((o)->type == HAW_TINT)
#define t_isnumber(o) ((o)->type == HAW_TNUMBER)
#define t_isstring(o) ((o)->type == HAW_TSTRING)
#define t_isvoid(o) ((o)->type == HAW_TVOID)
#define t_isvoid(o) ((o)->type == HAW_TVOID)
#define t_isvalid(o) ((o)->type > HAW_TNONE)
#define t_issame(a, b) ((a)->type == (b)->type)

#define t_isrational(o) (t_isnumber(o) || t_isint(o))

#define int_value(o) (o)->value_.int_
#define number_value(o) (o)->value_.number_
#define string_value(o) (o)->value_.str_

#define setnvalue(o, v) (o)->value_.number_ = v
#define setivalue(o, v) (o)->value_.int_ = v

static void print_value(const TValue* value)
{
	if (t_isint(value))
	{
		printf("%d", int_value(value));
	}
	else if (t_isnumber(value))
	{
		printf("%g", number_value(value));
	}
	else if (t_isstring(value))
	{
		String_print(string_value(value));
	}
	else if (t_isvoid(value))
	{
		printf("<void>");
	}
}

static inline haw_number val_to_num(TValue* v)
{
	return t_isint(v) ? (haw_number) int_value(v) : number_value(v);
}

#endif //! haw_value_h
