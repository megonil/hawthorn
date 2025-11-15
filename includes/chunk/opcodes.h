#ifndef haw_bytecode
#define haw_bytecode

#include "type/type.h"

#include <share/common.h>
#include <share/error.h>

#define MODULE_NAME "opcodes"

typedef enum : uint8_t
{
	OP_CONSTANT,
	OP_CONSTANT_LONG,

	OP_ADD,	 // +
	OP_MUL,	 // *
	OP_SUB,	 // -
	OP_DIV,	 // /
	OP_MOD,	 // %
	OP_IDIV, // //
	OP_NEG,	 // negate

	OP_SETLOCAL,
	OP_SETGLOBAL,
	OP_LOADLOCAL,
	OP_LOADGLOBAL,

	OP_CALL, // call function

	OP_RETURN,

	OP_LAST,
} OpCodes;

#define NUM_OPCODES ((int) OP_LAST)

static const char* opnames[NUM_OPCODES] = {
	[OP_CONSTANT]	   = "CONSTANT",
	[OP_CONSTANT_LONG] = "CONSTANT_LONG",
	[OP_ADD]		   = "ADD",
	[OP_MUL]		   = "MUL",
	[OP_SUB]		   = "SUB",
	[OP_DIV]		   = "DIV",
	[OP_MOD]		   = "MOD",
	[OP_IDIV]		   = "IDIV",
	[OP_NEG]		   = "NEG",
	[OP_SETLOCAL]	   = "SETLOCAL",
	[OP_SETGLOBAL]	   = "SETGLOBAL",
	[OP_LOADLOCAL]	   = "LOADLOCAL",
	[OP_LOADGLOBAL]	   = "LOADGLOBAL",
	[OP_CALL]		   = "CALL",
	[OP_RETURN]		   = "RETURN",
};

// push: how much values pushes to stack
// pop: how much values pops from stack
// jump: does manipulating ip
// has_arg: has argument
// use_k: does use constants
// ret: does manipulate on program execution
#define op_prop(push, pop, jump, has_arg, use_k, ret)                                              \
	(((pop) << 12) | ((push) << 8) | ((jump) << 4) | ((has_arg) << 3) | ((use_k) << 2) |           \
	 ((ret) << 1))

typedef uint16_t opprop_mask;

static const opprop_mask op_props[] = {
	[OP_ADD] = op_prop(2, 1, 0, 0, 0, 0),		[OP_MUL] = op_prop(2, 1, 0, 0, 0, 0),
	[OP_SUB] = op_prop(2, 1, 0, 0, 0, 0),		[OP_DIV] = op_prop(2, 1, 0, 0, 0, 0),
	[OP_IDIV] = op_prop(2, 1, 0, 0, 0, 0),		[OP_MOD] = op_prop(2, 1, 0, 0, 0, 0),
	[OP_NEG] = op_prop(1, 1, 0, 0, 0, 0),		[OP_RETURN] = op_prop(1, 0, 0, 0, 0, 1),
	[OP_CONSTANT] = op_prop(1, 0, 0, 1, 0, 0),	[OP_CONSTANT_LONG] = op_prop(1, 0, 0, 1, 0, 0),
	[OP_SETLOCAL] = op_prop(0, 2, 0, 1, 0, 0),	[OP_SETGLOBAL] = op_prop(0, 2, 0, 1, 0, 0),
	[OP_LOADLOCAL] = op_prop(1, 0, 0, 1, 0, 0), [OP_LOADGLOBAL] = op_prop(1, 0, 0, 1, 0, 0),
	[OP_CALL] = op_prop(0, 0, 0, 1, 0, 0),
};

#define op_numarg(op) ((op_props[op] >> 8) & 0xF)
#define op_hasflag(op, flag) (op_props[op] & (flag))

inline const char* op_name(OpCodes op)
{
	if (op <= NUM_OPCODES)
	{
		return opnames[op];
	}

	error("Unknown operator");
}

#endif
