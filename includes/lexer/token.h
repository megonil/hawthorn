#ifndef haw_token
#define haw_token

#include "share/string.h"

#include <limits.h>
#include <share/common.h>
#include <type/type.h>

// Single-char tokens (terminal symbols) are represented by their own numeric code. Other tokens
// start at the following value.
#define FIRST_RESERVED (UCHAR_MAX + 1)

typedef enum : uint16_t
{
	TK_RETURN = FIRST_RESERVED,
	TK_BREAK,

	// blocks
	TK_ELSE,
	TK_IF,
	TK_FUN, // function
	TK_PRO, // procedure

	// variables
	TK_LOCAL,
	TK_GLOBAL,

	TK_VOID,

	// cycles
	TK_DO,
	TK_WHILE,
	TK_FOR,

	// operators
	TK_AND,
	TK_OR,
	TK_NOTEQ, // !=
	TK_EQ,	  // ==
	TK_LE,
	TK_IDIV,

	// literals
	TK_BOOL,
	TK_INT,
	TK_NUMBER,
	TK_STRING
} TokenType;

typedef union
{
	haw_number num_;
	haw_int	   int_;
	str		   str_;
} SemInfo;

typedef struct
{
	TokenType type;
	SemInfo	  seminfo;
} Token;

typedef int32_t lexer_size;

typedef struct
{
	lexer_size line;
	lexer_size column;
} position;

static cstr const haw_tokens[] = {
	[TK_RETURN] = "return",	 [TK_BREAK] = "break",

	[TK_ELSE] = "else",		 [TK_IF] = "if",		 [TK_FUN] = "fun",
	[TK_PRO] = "pro",

	[TK_LOCAL] = "local",	 [TK_GLOBAL] = "global",

	[TK_VOID] = "void",

	[TK_DO] = "do",			 [TK_WHILE] = "while",	 [TK_FOR] = "for",

	[TK_AND] = "and",		 [TK_OR] = "or",		 [TK_NOTEQ] = "!=",
	[TK_EQ] = "==",			 [TK_LE] = "<=",		 [TK_IDIV] = "//",

	[TK_BOOL] = "<bool>",	 [TK_INT] = "<integer>", [TK_NUMBER] = "<number>",
	[TK_STRING] = "<string>"};

#endif // !haw_token
