#ifndef megonil_table
#define megonil_table

#include "share/string.h"

#include <stddef.h>
#include <stdlib.h>
static const float	LOAD_FACTOR	 = 0.5f;
static const size_t INITIAL_SIZE = 100;
static const float	MULTIPLIER	 = 2.0f;

typedef cstr_mut		   K;
typedef cstr_mut		   V;
typedef unsigned long long hash_code;

typedef struct Entry
{
	K key;
	V value;
} Entry;

inline void free_entry(Entry** e)
{
	free((*e)->key);
	free((*e)->value);
	free(*e);
	//*e = NULL;
}

#define hash(a) (hashcode((ucstr_mut) a))

#define CMP_EQ(a, b) (strcmp((a), (b)) == 0)

inline hash_code hashcode(ucstr str)
{
	hash_code hash = 5381;
	int		  c;

	while ((c = *str++))
	{
		hash = ((hash << 5) + hash) ^ c;
	}

	return hash;
}

typedef struct HashMap
{
	Entry** data;
	size_t	size; // quantity of elements in map
	size_t	arr_size;
	size_t	limit; // integer value of number of elements in map when rebuild happens
	float	load_factor;
	float	multiplier;
} HashMap;

HashMap* create_hashmap(size_t limit, float load_factor, float multiplier)
{
	HashMap* tmp = (HashMap*) malloc(sizeof(HashMap));

	tmp->arr_size = (limit >= INITIAL_SIZE) ? limit : INITIAL_SIZE;
	tmp->load_factor =
		(load_factor >= LOAD_FACTOR && load_factor <= 1.0f) ? load_factor : LOAD_FACTOR;
	tmp->limit		= (int) (tmp->load_factor * tmp->arr_size);
	tmp->size		= 0;
	tmp->multiplier = (multiplier >= MULTIPLIER) ? multiplier : MULTIPLIER;

	tmp->data = (Entry**) calloc(tmp->arr_size, sizeof(Entry*));
	return tmp;
}

HashMap* hashmap_rehashup(HashMap** _map_, Entry* e);

void hashmap_rawput(HashMap** _map_, Entry* e)
{
	HashMap*  map  = *_map_;
	hash_code hash = hash(e->key);

	size_t index = (hash * map->arr_size);

	if (map->size <= map->limit)
	{
		if (map->data[index] == NULL)
		{
			map->data[index] = e;
		}
		else
		{
			while (map->data[index])
			{
				index++;
				if (index >= map->arr_size)
				{
					index = 0;
				}
			}
			map->data[index] = e;
		}
	}

	else
	{
		*_map_ = hashmap_rehashup(_map_, e);
	}

	(*_map_)->size++;
}

void hashmap_put(HashMap** map, K key, V value)
{
	Entry* e = (Entry*) malloc(sizeof(Entry));

	e->key	 = key;
	e->value = value;

	hashmap_rawput(map, e);
}

HashMap* hashmap_rehashup(HashMap** _map_, Entry* e)
{
	HashMap* new_map = create_hashmap((size_t) (*_map_)->arr_size * (*_map_)->multiplier,
									  (*_map_)->load_factor, (*_map_)->multiplier);
	size_t	 i, size;

	HashMap* map = (*_map_);

	size = (*_map_)->arr_size;
	for (i = 0; i < size; i++)
	{
		if (map->data[i])
		{
			hashmap_rawput(&new_map, map->data[i]);
		}
	}

	free(map->data);
	free(*_map_);
	hashmap_rawput(&new_map, e);
	return new_map;
}

void hashmap_destroy(HashMap** _map_)
{
	HashMap* map = *_map_;
	size_t	 i, size;

	size = map->arr_size;
	for (i = 0; i < size; i++)
	{
		if (map->data[i])
		{
			free_entry(&(map->data[i]));
		}
	}

	free(map->data);
	free(*_map_);
	*_map_ = NULL;
}

V hashmap_get(HashMap* map, K key)
{
	hash_code hash	 = hash(key);
	size_t	  index	 = (hash % map->arr_size);
	V		  result = NULL;
	if (map->data[index])
	{
		if (CMP_EQ(map->data[index]->key, key))
		{
			result = map->data[index]->value;
		}
		else
		{
			while (map->data[index] == NULL || !CMP_EQ(map->data[index]->key, key))
			{
				index++;
				if (index >= map->arr_size)
				{
					index = 0;
				}
			}
			result = map->data[index]->value;
		}
	}

	return result;
}
Entry* xremove(HashMap* map, K key)
{
	unsigned long long hash	  = hash(key);
	size_t			   index  = (hash % map->arr_size);
	Entry*			   result = NULL;

	if (map->data[index] != NULL)
	{
		if (CMP_EQ(map->data[index]->key, key))
		{
			result			 = map->data[index];
			map->data[index] = NULL;
		}
		else
		{
			while (!CMP_EQ(map->data[index]->key, key))
			{
				index++;
				if (index >= map->arr_size)
				{
					index = 0;
				}
			}
			result			 = map->data[index];
			map->data[index] = NULL;
		}
	}

	map->size--;
	return result;
}
#endif // !megonil_table
