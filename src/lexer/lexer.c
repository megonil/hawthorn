#include "lexer/token.h"
#include "type/type.h"
#include "value/value.h"

#include <assert.h>
#include <ctype.h>
#include <lexer/lexer.h>
#include <share/array.h>
#include <share/error.h>
#include <share/hawthorn.h>
#include <share/string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MODULE_NAME "lexer"

#undef this
#define this SynLexState* sls

#define save(sls, c) String_appendc(&sls->buffer, c);
// #define save(sls, c) String_appendc(&sls->buffer, c)
#define save_and_next(sls)                                                                         \
	do                                                                                             \
	{                                                                                              \
		save(sls, sls->current);                                                                   \
		advance(sls);                                                                              \
	} while (0)

static inline void advance(this)
{
	if (sls->file_contents.n + 1 < ((((ArrayHeader*) (sls->file_contents.value) - 1)->size)))
	{
		sls->current = buffer_readnext(sls->file_contents);
	}
	else
	{
		sls->current = EOF;
	}
}

static noret lex_error(this, cstr msg, TokenType token);

static lexer_char check_next1(this, lexer_char c)
{
	if (sls->current == c)
	{
		advance(sls);
		return 1;
	}

	return 0;
}

static lexer_char is_reserved(const String* str)
{
	const char* s  = str->value;
	size_t		ln = (size_t) str->length;
	if (ln == 0) return (TokenType) 0;
#define KW_CHECK_PART(start, rest_lit, tok)                                                        \
	do                                                                                             \
	{                                                                                              \
		const size_t __kw_len = (sizeof(rest_lit) - 1u);                                           \
		if (ln == (size_t) (start) + __kw_len && memcmp(s + (start), (rest_lit), __kw_len) == 0)   \
			return (tok);                                                                          \
	} while (0)
#define KW_CHECK_FULL(rest_lit, tok) KW_CHECK_PART(0, rest_lit, tok)
	switch ((unsigned char) s[0])
	{
	case 'a':
		KW_CHECK_FULL("and", TK_AND);
		break;
	case 'b':
		if (ln > 1)
		{
			switch (s[1])
			{
			case 'r':
				KW_CHECK_PART(2, "eak", TK_BREAK);
				break;
			}
		}
		break;
	case 'd':
		KW_CHECK_FULL("do", TK_DO);
		break;
	case 'e':
		KW_CHECK_FULL("else", TK_ELSE);
		break;
	case 'f':
		if (ln > 1)
		{
			switch (s[1])
			{
			case 'o':
				KW_CHECK_PART(1, "or", TK_FOR);
				break;
			case 'u':
				KW_CHECK_PART(2, "n", TK_FUN);
				break;
			case 'a':
				KW_CHECK_PART(2, "lse", TK_BOOL);
				break;
			}
		}
		break;
	case 'g':
		KW_CHECK_FULL("global", TK_GLOBAL);
		break;
	case 'i':
		if (ln > 1)
		{
			switch (s[1])
			{
			case 'f':
				KW_CHECK_FULL("if", TK_IF);
				break;
			case 'n':
				KW_CHECK_PART(1, "nt", TK_INT);
				break;
			}
		}
		break;
	case 'l':
		KW_CHECK_FULL("local", TK_LOCAL);
		break;
	case 'o':
		KW_CHECK_FULL("or", TK_OR);
		break;
	case 'p':
		KW_CHECK_FULL("pro", TK_PRO);
		break;
	case 'r':
		KW_CHECK_FULL("return", TK_RETURN);
		break;
	case 'v':
		KW_CHECK_FULL("void", TK_VOID);
		break;
	case 'w':
		KW_CHECK_FULL("while", TK_WHILE);
		break;
	case 't':
		if (ln > 1)
		{
			switch (s[1])
			{
			case 'r':
				KW_CHECK_PART(2, "ue", TK_BOOL);
				break;
			}
		}
		break;
	default:
		break;
	}
#undef KW_CHECK_PART
#undef KW_CHECK_FULL
	return 0;
}
static lexer_char check_next2(this, const char* set)
{
	assert(set[2] == '0');
	if (sls->current == set[0] || sls->current == set[1])
	{
		save_and_next(sls);
		return 1;
	}
	return 0;
}

static lexer_char keyword_or_name(this)
{
	lexer_char ch = is_reserved(&sls->buffer);

	if (ch == 0)
	{
		return TK_NAME;
	}

	return ch;
}

static int isneg(cstr* string)
{
	if (**string == '-')
	{
		(*string)++;
		return 1;
	}
	else if (**string == '+')
	{
		(*string)++;
	}

	return 0;
}

// 1 -> int
// 2 -> float
static int str_2num(const char* s, TValue* result)
{
#define next() p++

	int dot = 0;

	const char* p = s;

	if (*p == '+' || *p == '-')
	{
		next();
	}

	massert(isdigit(*p), "Numbers must be starting with number, found '%c'", *p);

	for (; isdigit(*p) || *p == '.'; next())
	{
		if (*p == '.')
		{
			if (dot)
			{
				error("Multiple dots in number");
			}
			dot = 1;
		}
	}

	if (isalpha(*p))
	{
		error("Number touching a letter");
	}

	if (!dot) // just integer value
	{
		haw_int i = strtoll(s, NULL, 10);
		setivalue(result, i);
		return 1;
	}

	// instead haw_number
	haw_number n = strtod(s, NULL);
	setnvalue(result, n);

	return 2;

#undef next
}

static void read_string(this, SemInfo* seminfo)
{
	assert(sls->current == '"');
	advance(sls); // "

	while (sls->current != '"')
	{
		switch (sls->current)
		{
		case EOF:
		case '\n':
		case '\r':
			error("Unfinished string");
			break;
		default:
			save_and_next(sls);
		}
	}
	advance(sls); // "

	make_String(seminfo->str_, sls->buffer.value);
}

static lexer_char read_numeral(this, SemInfo* seminfo)
{
	assert(isdigit(sls->current));
	save_and_next(sls);

	while (isdigit(sls->current) || sls->current == '.')
	{
		save_and_next(sls);
	}

	if (isalpha(sls->current))
	{
		error("Number touching a letter");
	}

	TValue obj;
	int	   t = str_2num(sls->buffer.value, &obj);

	if (t == 1)
	{
		seminfo->int_ = int_value(&obj);
		return TK_INT;
	}
	else
	{
		seminfo->num_ = number_value(&obj);
		return TK_NUMBER;
	}
}

void synlex_init(this, str* source_name)
{
	sls->source_name = source_name;

	buffer_init(&sls->file_contents);
	buffer_readfile(&sls->file_contents, source_name->value);

	sls->current = buffer_readnext(sls->file_contents);
	String_init(&sls->buffer);
}

uint8_t current_is_new_line(this)
{
	return sls->current == '\n' || sls->current == '\r';
}

void synlex_ttype_to_str(lexer_char token, cstr_mut destiny)
{
	if (token < FIRST_RESERVED) // single byte symbols?
	{
		if (isprint(token))
		{
			sprintf(destiny, "%c", token);
		}
		else
		{
			sprintf(destiny, "\\%d", token);
		}
	}
	else
	{
		const char* s = haw_tokens[token - FIRST_RESERVED];
		sprintf(destiny, "%s", s);
	}
}

Token synlex_lex(this)
{
#define result_tset(t)                                                                             \
	result.type = t;                                                                               \
	goto done;
	SemInfo seminfo;
	Token	result;
	String_clear(&sls->buffer); // clear buffer

	for (;;)
	{
		// #if SLS_DEBUGL
		// 		printf("`%c` ", sls->current);
		// #endif
		switch (sls->current)
		{
		case EOF:
			result_tset(TK_EOF);
			break;
		case '\n':
		case '\r':
			sls->line_number++;
			advance(sls);
			break;
		case ' ':
		case '\t':
		case '\f':
		case '\v':
			advance(sls);
			break;
		case '#': // comment
			while (!current_is_new_line(sls) && sls->current != EOF)
			{
				advance(sls);
			}
			break;
		case '=':
			advance(sls);
			if (check_next1(sls, '=')) // ==
			{
				result_tset(TK_EQ);
			}
			else
			{
				result_tset('=');
			}
			break;

		case '<':
			advance(sls);
			if (check_next1(sls, '=')) // <=
			{
				result_tset(TK_LE);
			}
			else
			{
				result_tset('<');
			}
			break;

		case '>':
			advance(sls);
			if (check_next1(sls, '=')) // >=
			{
				result_tset(TK_GE);
			}
			else
			{
				result_tset('>');
			}
			break;
		case '!':
			advance(sls);
			if (check_next1(sls, '=')) // !=
			{
				result_tset(TK_NOTEQ);
			}
			else
			{
				result_tset('!');
			}
			break;
		case '"':
			read_string(sls, &seminfo);
			result_tset(TK_STRING);
			break;
		case '\'':
			advance(sls); // '

			if (check_next1(sls, '\''))
			{
				error("Expected char");
			}
			save_and_next(sls); // save the char payload

			if (!check_next1(sls, '\''))
			{
				error("Expected end of char");
			}

			advance(sls); // '
			result_tset(TK_CHAR);
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			result_tset(read_numeral(sls, &seminfo));
			break;
		}
		default:
		{
			if (isalpha(sls->current) || sls->current == '_')
			{
				save_and_next(sls);
				while (isalnum(sls->current) || sls->current == '_')
				{
					save_and_next(sls);
				}

				result_tset(keyword_or_name(sls));
			}
			else
			{
				// single char tokens
				lexer_char c = sls->current;
				advance(sls);

				result_tset(c);
			}
		}
		break;
		}
	}

done:
	result.seminfo = seminfo;
	return result;

#undef result_tset
}

void synlex_destroy(this)
{
	String_destroy(&sls->buffer);
	buffer_destroy(&sls->file_contents);
}

void synlex_dislex(lexer_char token)
{
	if (token < FIRST_RESERVED) // single byte symbols?
	{
		if (isprint(token))
		{
			printf("%c ", token);
		}
		else
		{
			printf("\\%d", token);
		}
	}
	else
	{
		const char* s = haw_tokens[token - FIRST_RESERVED];
		printf("%s ", s);
	}
}

cstr_mut tok_2str(cstr_mut s, lexer_char token)
{
	if (token < FIRST_RESERVED) // single byte symbols?
	{
		if (isprint(token))
		{
			sprintf(s, "%c", token);
		}
		else
		{
			sprintf(s, "\\%d", token);
		}
	}
	else
	{
		const char* str = haw_tokens[token - FIRST_RESERVED];
		sprintf(s, "%s", str);
	}

	return s;
}

#undef assignl
#undef this
