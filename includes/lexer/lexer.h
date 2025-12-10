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
	str*	   source_name;
	SemInfo*   seminfo;
} SynLexState;

// specify to print or not token which is currently being processed
#define SLS_DEBUGL 1

#define this SynLexState* sls

HAWI_FUNC void	synlex_init(this, str* source_name, SemInfo* seminfo);
HAWI_FUNC void	synlex_setinput(this, str* source_name);
HAWI_FUNC void	synlex_next(this);
HAWI_FUNC noret synlex_syntaxerror(this, cstr message);
HAWI_FUNC noret synlex_lexerror(this, cstr message);
HAWI_FUNC cstr	synlex_tokentostr(this, TokenType token);

Token synlex_lex(this);
void  synlex_destroy(this);

#if SLS_DEBUGL

void	 synlex_dislex(this, lexer_char token);
cstr_mut tok_2str(cstr_mut s, lexer_char token);

#endif

#endif // !haw_lexer
#undef this
