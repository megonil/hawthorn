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
	// TODO
	case TK_RETURN:
	case TK_BREAK:
	case TK_ELSE:
	case TK_IF:
	case TK_FUN:
	case TK_PRO:
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
	case TK_VOID:
	case TK_NAME:
	case TK_EOF:
	case TK_BIND:
	case TK_SET:
		break;

	case TK_INT:
		printf("(%d) ", p.sls->seminfo->int_);
		break;

	case TK_NUMBER:
		printf("(%lf) ", p.sls->seminfo->num_);
		break;

	case TK_STRING:
		String_printd(p.current.seminfo.str_);
		break;
	case TK_CHAR:
		printf("('%c')", *p.sls->seminfo->str_->value);
		break;
	}

	next();
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
	SemInfo seminfo;
	synlex_init(p.sls, filename, &seminfo);

	for (next(); p.current.type != TK_EOF; stmt())
	{
		synlex_dislex(p.sls, p.current.type);
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
