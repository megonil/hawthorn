#ifndef hawthorn_chunk
#define hawthorn_chunk

#include <share/array.h>
#include <share/common.h>
#include <stdint.h>
#include <type/type.h>
#include <value/value.h>

typedef TValue Constant;

typedef struct
{
	hawu_byte* code;
	Constant*  constants;
} Chunk;

// util
// will be undefined
#define this Chunk* chunk

#define MAX_CONSTANT_LEN UINT8_MAX
#define BYTE 0xFF
#define BYTE_DEC 8

#define pass_chunk(c) (&c)

void	 chunk_init(this);
uint32_t write_constant(this, Constant data);

void emit_byte(this, hawu_byte byte);

void chunk_destroy(this);

void disassemble(this);

#define emit(v) (emit_byte(chunk, v))

#endif // !hawthorn_chunk
