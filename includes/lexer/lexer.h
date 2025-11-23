#ifndef haw_lexer
#define haw_lexer

#include "share/string.h"
#include "token.h"

#include <share/hawthorn.h>
#include <share/util.h>

typedef int lexer_char;

/// State of the scanner plus state of the parser when shared by all functions
typedef struct
{
	// LEXER
	Buffer	   file_contents;
	lexer_char current;
	lexer_char look_ahead;
	lexer_size line_number;
	String	   buffer;

	// PARSER
	Token  current_token;	 // current token
	Token  look_ahead_token; // next token
	size_t pos;

	// BOTH
	str* source_name;
} SynLexState;

// specify to print or not token which is currently being processed
#define SLS_DEBUGL 1

#define this SynLexState* sls

HAWI_FUNC void	synlex_init(this, str* source_name);
HAWI_FUNC void	synlex_setinput(this, str* source_name);
HAWI_FUNC void	synlex_next(this);
HAWI_FUNC noret synlex_syntaxerror(this, cstr message);
HAWI_FUNC noret synlex_lexerror(this, cstr message);
HAWI_FUNC cstr	synlex_tokentostr(this, TokenType token);

lexer_char synlex_lex(this, SemInfo* seminfo);
void	   synlex_destroy(this);

#if SLS_DEBUGL

void synlex_dislex(lexer_char token);

#endif

#endif // !haw_lexer
