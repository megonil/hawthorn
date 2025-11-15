#include "interpreter/repl.h"
#include "lexer/lexer.h"

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		repl_cycle();
	}
	else if (argc == 2)
	{
		SynLexState sl;
		str			source_name = make_str(argv[1]);
		synlex_init(&sl, &source_name);
		synlex_lex(&sl);
	}

	return 0;
}
