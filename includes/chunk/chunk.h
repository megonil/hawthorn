#ifndef hawthorn_chunk
#define hawthorn_chunk

#include <share/array.h>
#include <share/common.h>
#include <type/type.h>
#include <value/value.h>

typedef Value Constant;

typedef struct
{
	hawu_byte* code;
	Constant*  constants;
} Chunk;

// util
// will be undefined
#define this Chunk* chunk
#define this_t Chunk*

#define MAX_CONSTANT_LEN UINT8_MAX

#define BYTE 0xFF
#define BYTE_DEC 8

#define pass_chunk(c) (&c)

void   init_chunk(this);
size_t write_constant(this, Value data);

inline void emit_byte(this, hawu_byte byte)
{
	array_push(chunk->code, byte);
}

inline void free_chunk(this)
{
	array_free(chunk->code);
	array_free(chunk->constants);
}

void disassemble(this);

#define emit(v) (emit_byte(chunk, v))

#endif // !hawthorn_chunk
