#include "chunk/opcodes.h"
#include "share/array.h"
#include "type/type.h"

#include <assert.h>
#include <chunk/chunk.h>
#include <stdio.h>

void init_chunk(this)
{
	chunk->code		 = array(hawu_byte); // initializing code array
	chunk->constants = array(Constant);	 // initializing value array
};

static size_t add_constant(this, Constant constant)
{
	array_push(chunk->constants, constant);

	return array_size(chunk->constants);
}

size_t write_constant(this, Constant data)
{
	auto index = add_constant(chunk, data);

	if (index < MAX_CONSTANT_LEN)
	{
		emit(OP_CONSTANT);
		emit(index);
		return add_constant(chunk, data);
	}

	auto uindex = cast(uint32_t, index);

	emit(OP_CONSTANT_LONG);
	emit(cast_hawsbyte(uindex & BYTE));
	emit(cast_hawsbyte((uindex >> BYTE_DEC)) & BYTE);
	emit(cast_hawsbyte((uindex >> 2 * BYTE_DEC)) & BYTE);

	return index;
}

void disassemble(this)
{
	for (size_t i = 0; i < array_size(chunk->code); i++)
	{
		printf("%s", op_name(array_at(chunk->code, i, hawu_byte)));
	}
}

#undef this	  // Chunk* chunk
#undef this_t // Chunk
