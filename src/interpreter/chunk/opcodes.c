#include "chunk/opcodes.h"

#include "share/error.h"

const char* op_name(OpCodes op)
{
	if (op <= NUM_OPCODES)
	{
		return opnames[op];
	}

	error("Unknown operator");
}
