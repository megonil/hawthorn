#include "lexer/lexer.h"
#include "lexer/token.h"

#include <parser/parser.h>
#include <stdio.h>

void parse(str filename)
{
	SynLexState sls;
	synlex_init(&sls, &filename);

	SemInfo	   dummy;
	lexer_char current;

	while ((current = synlex_lex(&sls, &dummy)) != EOF)
	{
		synlex_dislex(current);
	}

	printf("\n"); // END debug seq
}
