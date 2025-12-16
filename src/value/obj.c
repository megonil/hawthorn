#include "share/table.h"
#include "value/value.h"

#include <interpreter/vm.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <value/obj.h>
#define endstring(s, l) (s)[(l)] = '\0'

static uint32_t hash_string(const char* key, int length)
{
	uint32_t hash = 2166136261u;

	for (int i = 0; i < length; i++)
	{
		hash ^= (uint8_t) key[i];
		hash *= 16777619;
	}

	return hash;
}

Obj* allocate_object(size_t size, ObjType type)
{
	Obj* object = (Obj*) malloc(size);

	object->type = type;
	object->next = v.objects;
	v.objects	 = object;

	return object;
}

static haw_string* allocate_string(char* chars, int length, hash hash)
{
	haw_string* string =
		allocate_obj_fam(sizeof(haw_string) + sizeof(char) * length, haw_string, OBJ_STRING);

	string->length = length;
	string->chars  = chars;
	string->hash   = hash;
	table_set(&v.strings, string, (TValue) {.value_ = (Value) {.number_ = 0}, .type = HAW_TNONE});

	return string;
}

haw_string* take_string(char* chars, int length)
{
	hash		hash	 = hash_string(chars, length);
	haw_string* interned = table_find_string(&v.strings, chars, length, hash);

	if (interned != NULL)
	{
		return interned;
	}

	return allocate_string(chars, length, hash);
}

haw_string* copy_string(const char* chars, int length)
{
	hash		hash	 = hash_string(chars, length);
	haw_string* interned = table_find_string(&v.strings, chars, length, hash);

	if (interned != NULL)
	{
		return interned;
	}

	char* new_chars = allocate(char, length + 1);

	memcpy(new_chars, chars, length);
	new_chars[length] = '\0';

	return allocate_string(new_chars, length, hash);
}

haw_string* concatenate(haw_string* a, haw_string* b)
{
	int			length	   = a->length + b->length;
	size_t		chars_size = sizeof(char) * (length + 1);
	haw_string* string =
		allocate_obj_fam(sizeof(*string) + chars_size, haw_string, OBJ_STRING); // allocation

	string->length = length;
	string->chars  = (char*) (string + 1);

	memcpy(string->chars, a->chars, a->length);				// copy
	memcpy(string->chars + a->length, b->chars, b->length); // copy

	endstring(string->chars, length);

	return string;
}

void free_object(Obj* obj)
{
	switch (obj->type)
	{
	case OBJ_STRING:
		haw_string* string = cast_string(obj);
		free(string);

		break;
	}
}

void free_objects()
{
	Obj* object = v.objects;

	while (object != NULL)
	{
		Obj* next = object->next;
		free_object(object);
		object = next;
	}
}
