#include "interpreter/vm.h"

#include "chunk/chunk.h"
#include "chunk/opcodes.h"
#include "lexer/lexer.h"
#include "share/array.h"
#include "type/type.h"
#include "value/value.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

vm v;

#undef MODULE_NAME
#define MODULE_NAME "vm"

#define pop() array_pop(v.stack, TValue)
#define push(valu) array_push(v.stack, valu)

#define read_byte() v.chunk->code[v.pc++]

#define wrongoperandsc(operand) errorf("Wrong operands to binary operator `%c`", operand)

void vm_init(Chunk* chunk)
{
	v.stack = array(TValue);
	v.chunk = chunk;
	v.pc	= 0;
}

size_t chunk_size()
{
	size_t size = array_size(v.chunk->code);

	return size;
}

void vm_execute()
{
	for (;;)
	{
		uint8_t instruction = read_byte();
		if (instruction == OP_HALT)
		{
			return;
		}

		switch (instruction)
		{
		case OP_CONSTANT:
		{
			uint8_t index = read_byte();
			push(v.chunk->constants[index]);
			break;
		}
		case OP_CONSTANT_LONG:
		{
			uint8_t	 major = read_byte();
			uint8_t	 mid   = read_byte();
			uint8_t	 minor = read_byte();
			uint32_t index = major | (uint32_t) mid << 8 | (uint32_t) minor << 16;
			push(v.chunk->constants[index]);
			break;
		}
		case OP_PRINT:
		{
			print_value(&pop());
			printf("\n");
			break;
		}

		// Binary Operators
#define macrostart()                                                                               \
	TValue result;                                                                                 \
	TValue b = pop();                                                                              \
	TValue a = pop()

#define macroend()                                                                                 \
	push(result);                                                                                  \
	break;

#define binopr(t, op, ch)                                                                          \
	case t:                                                                                        \
	{                                                                                              \
		macrostart();                                                                              \
                                                                                                   \
		if (t_isrational(&a) && t_isrational(&b))                                                  \
		{                                                                                          \
			if (t_isint(&a) && t_isint(&b))                                                        \
			{                                                                                      \
				result.type = HAW_TINT;                                                            \
				setivalue(&result, int_value(&a) op int_value(&b));                                \
			}                                                                                      \
			else                                                                                   \
			{                                                                                      \
				result.type = HAW_TNUMBER;                                                         \
				setnvalue(&result, val_to_num(&a) op val_to_num(&b));                              \
			}                                                                                      \
		}                                                                                          \
		else                                                                                       \
		{                                                                                          \
			wrongoperandsc(ch);                                                                    \
		}                                                                                          \
		macroend();                                                                                \
	}
			binopr(OP_ADD, +, '+');
			binopr(OP_SUB, -, '-');
			binopr(OP_MUL, *, '*');
			binopr(OP_DIV, /, '/');

		case OP_MOD:
		{
			macrostart();

			if (t_isint(&a) && t_isint(&b))
			{
				result.type = HAW_TINT;
				setivalue(&result, int_value(&a) % int_value(&b));
			}
			else if (t_isrational(&a) && t_isrational(&b))
			{
				result.type = HAW_TNUMBER;
				setnvalue(&result, fmod(val_to_num(&a), val_to_num(&b)));
			}
			else
			{
				wrongoperandsc('%');
			}
			macroend();
		}
		case OP_IDIV:
		{
			macrostart();

			if (t_isint(&a) && t_isint(&b))
			{
				result.type = HAW_TINT;
				setivalue(&result, cast_hawint(int_value(&a) / int_value(&b)));
			}
			else if (t_isrational(&a) && t_isrational(&b))
			{
				result.type = HAW_TNUMBER;
				setnvalue(&result, fdiv(val_to_num(&a), val_to_num(&b)));
			}
			else
			{
				error("Wrong operands to binary operator `//`");
			}

			macroend();
		}
		case OP_POW:
		{
			macrostart();

			if (t_isrational(&a) && t_isrational(&b))
			{
				result.type = HAW_TNUMBER;
				setnvalue(&result, pow(val_to_num(&a), val_to_num(&b)));
			}

			else
			{
				wrongoperandsc('^');
			}

			macroend();
		}

#define boolopr(t, op)                                                                             \
	case t:                                                                                        \
	{                                                                                              \
		macrostart();                                                                              \
                                                                                                   \
		if (t_isint(&a) && t_isint(&b))                                                            \
		{                                                                                          \
			result.type = HAW_TINT;                                                                \
			setivalue(&result, int_value(&a) op int_value(&b));                                    \
		}                                                                                          \
		else if (t_isrational(&a) && t_isrational(&b))                                             \
		{                                                                                          \
			result.type = HAW_TINT;                                                                \
			setivalue(&result, val_to_num(&a) op val_to_num(&b));                                  \
		}                                                                                          \
		else                                                                                       \
		{                                                                                          \
			error("Wrong operands to binary operator `==`");                                       \
		}                                                                                          \
                                                                                                   \
		macroend();                                                                                \
	}

			boolopr(OP_EQ, ==);
			boolopr(OP_GE, >=);
			boolopr(OP_GT, >);
			boolopr(OP_LE, <=);
			boolopr(OP_LT, <);
			boolopr(OP_AND, &&);
			boolopr(OP_OR, ||);

#undef binopr
#undef boolopr
#undef macrostart
#undef macroend
		}
	}
}

void vm_destroy()
{
	chunk_destroy(v.chunk);
}

#undef pop
#undef push
