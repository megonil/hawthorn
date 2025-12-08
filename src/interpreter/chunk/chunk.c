#include "chunk/opcodes.h"
#include "type/type.h"
#include "value/value.h"

#include <chunk/chunk.h>
#include <stdio.h>

void chunk_init(Chunk* chunk)
{
	chunk->code		 = array(hawu_byte);
	chunk->constants = array(TValue);
}

void emit_byte(this, hawu_byte byte)
{
	array_push(chunk->code, byte);
}

void chunk_destroy(this)
{
	array_free(chunk->code);
	array_free(chunk->constants);
}

static void print_value(const TValue* v)
{
	if (tt_isint(v))
	{
		printf("%lld", (long long) int_value(v));
	}
	else if (tt_isnumber(v))
	{
		printf("%f", (double) number_value(v));
	}
	else
	{
		printf("<unknown type %d>", rawtt(v));
	}
}
static int disassemble_instruction(Chunk* chunk, int offset)
{
	uint8_t op = chunk->code[offset];

	printf("%04d  %-16s", offset, op_name(op));

	int next = offset + 1;

	if (op_hasflag(op, (1 << 3)))
	{
		uint32_t arg = 0;

		if (op == OP_CONSTANT_LONG)
		{
			arg = (chunk->code[next] << 16) | (chunk->code[next + 1] << 8) | chunk->code[next + 2];
			next += 3;
		}
		else
		{
			arg = chunk->code[next++];
		}

		printf(" arg=%u", arg);

		if (op_hasflag(op, (1 << 2)))
		{
			printf(" ; const=");
			print_value(&chunk->constants[arg]);
		}
	}

	int push = (op_props[op] >> 8) & 0xF;
	int pop	 = (op_props[op] >> 12) & 0xF;

	printf(" ; pop=%d push=%d", pop, push);

	if (op_hasflag(op, (1 << 1))) printf(" ret");

	if (op_hasflag(op, (1 << 4))) printf(" jump");

	printf("\n");

	return next;
}

void disassemble(Chunk* chunk)
{
	printf("=== disassembly ===\n");

	for (int offset = 0; offset < array_size(chunk->code); offset++)
	{
		offset = disassemble_instruction(chunk, offset);
	}

	printf("=== end ===\n");
}
