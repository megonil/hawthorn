#include "lexer/token.h"
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

#define MODULE_NAME "lexer"

#define peek(i) (sls->file_contents.value[sls->pos + i])
#define advance(sls)                                                                               \
	sls->current	= sls->file_contents.value[sls->pos++];                                        \
	sls->look_ahead = peek(1);
#define push

cstr get_contents_of_file(cstr file_name)
{
	FILE* file = fopen(file_name, "rb");

	if (file == NULL)
	{
		error("Could not open file");
	}

	fseek(file, 0L, SEEK_END);
	size_t file_size = ftell(file);
	rewind(file);

	char* buffer = (char*) malloc(file_size + 1);

	if (buffer == NULL)
	{
		error("Error allocating memory to file buffer");
	}

	size_t bytes_read = fread(buffer, sizeof(char), file_size, file);

	if (bytes_read < file_size)
	{
		errorf("Could not open file %s", file_name);
	}

	fclose(file);

	return buffer;
}

static noret	  lex_error(this, cstr msg, TokenType token);
static lexer_char check_next2(this, const char* set)
{
	assert(set[2] == '0');
	if (sls->current == set[0] || sls->current == set[1])
	{
		save_and_next();
	}
}
static lexer_char read_numeral(this, SemInfo* info)
{
	TValue		obj;
	const char* expo = "Ee";

	lexer_char first = sls->current;
	assert(isdigit(sls->current));
	advance(sls);

	if (first == '0' &&) }

void synlex_init(this, str* source_name)
{
	sls->source_name   = source_name;
	sls->file_contents = make_str(get_contents_of_file(source_name->value));
	sls->current	   = sls->file_contents.value[0];
}

inline uint8_t current_is_new_line(this)
{
	return sls->current == '\n' || sls->current == '\r';
}

lexer_char synlex_lex(this, SemInfo* seminfo)
{

	for (;;)
	{
		switch (sls->current)
		{
		case '\n':
		case '\r':
			sls->line_number++;
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
			if (sls->look_ahead == '=')
			{
				return TK_EQ;
			}
			return '=';
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
			return read_numeral(sls, seminfo);
		}
		}
	}
}

#undef assignl
