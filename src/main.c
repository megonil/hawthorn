#include "interpreter/repl.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "share/string.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		repl_cycle();
	}
	else if (argc == 2)
	{
		str source_name = make_str(argv[1]);
		parse(source_name);
	}

	return 0;
}
