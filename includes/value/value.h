#ifndef haw_value_h
#define haw_value_h

#include "share/string.h"

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

#define int_value(o) (o)->value_.int_
#define number_value(o) (o)->value_.number_
#define string_value(o) (o)->value_.str_

#define setnvalue(o, v) (o)->value_.number_ = v
#define setivalue(o, v) (o)->value_.int_ = v

#endif //! haw_value_h
