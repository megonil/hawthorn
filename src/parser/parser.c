#undef this
#include <chunk/chunk.h>
#include <lexer/lexer.h>
#include <lexer/token.h>
#include <share/error.h>
#include <share/string.h>

#define MODULE_NAME "parser"

#include <parser/parser.h>
#include <stdio.h>

this;

#define set_globalp(pa) p = pa

static void next()
{
	p.previous = p.current;
	p.current  = synlex_lex(p.sls);
}

static void consume(TokenType type, cstr msg)
{
	if (p.current.type == type)
	{
		next();
	}
	errorf("Expected %s, found %s", tok_2str("", type), tok_2str("", type));
}

// Statements
#define dstmt(s) static void s()
dstmt(stmt);
dstmt(vardecl);

// Expressions
#define dexpr(s) static void s()
dexpr(expr);
dexpr(intlit);
dexpr(numlit);
dexpr(strlit);
dexpr(varexpr);

#undef dexpr
#undef dstmt

#define expected(t) errorf("Expected ", tok_2str(t))

static void stmt()
{
	switch (p.current.type)
	{
	case TK_STRING:
		String_print(p.current.seminfo.str_);
		next();
		return;

	// TODO
	case TK_RETURN:
	case TK_BREAK:
	case TK_ELSE:
	case TK_IF:
	case TK_FUN:
	case TK_PRO:
	case TK_LOCAL:
	case TK_GLOBAL:
	case TK_DO:
	case TK_WHILE:
	case TK_FOR:
	case TK_AND:
	case TK_OR:
	case TK_NOTEQ:
	case TK_EQ:
	case TK_LE:
	case TK_GE:
	case TK_IDIV:
	case TK_BOOL:
	case TK_INT:
	case TK_NUMBER:
	case TK_CHAR:
	case TK_VOID:
	case TK_NAME:
	case TK_EOF:
		next();
		return;
	}
}

void parser_init(Parser* p, SynLexState* sls)
{
	p->sls		   = sls;
	p->scopes_deep = 0;

	chunk_init(&p->chunk);
	p->vars = array(VarDesc);
}

void parse(str* filename)
{
	synlex_init(p.sls, filename);
	p.current = synlex_lex(p.sls);

	while (p.current.type != TK_EOF)
	{
#ifdef SLS_DEBUGL
		synlex_dislex(p.current.type);
#endif
		stmt();
	}

	printf("\n"); // END debug seq
	// String_destroy(dummy.str_);
	synlex_destroy(p.sls);
}

void parser_destroy(Parser* p)
{
	chunk_destroy(&p->chunk);
	synlex_destroy(p->sls);
	array_free(p->vars);
}

#undef this
