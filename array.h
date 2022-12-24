#include <stdlib.h>

#define II__make_room(array, count)                                            \
{                                                                              \
	array.length = count;                                                      \
	array.capacity = count * 2;                                                \
	array.elements = malloc(sizeof(typeof(*array.elements)) * array.capacity); \
}

#define II__move_room(array, count)                                                             \
{                                                                                               \
	array.capacity = count * 2;                                                                 \
	array.elements = realloc(array.elements, sizeof(typeof(*array.elements)) * array.capacity); \
}

#define declare_array(type)    \
typedef struct                 \
{                              \
	void(*element_free)(type); \
	size_t capacity;           \
	size_t length;             \
	type* elements;            \
} I##type##I;                  \

#define II__init(array, free, ...)                                 \
do {                                                               \
	typeof(*array.elements) parameters[] = {__VA_ARGS__};          \
	size_t count = sizeof(parameters)/sizeof(typeof(*parameters)); \
                                                                   \
	array.element_free = (typeof(array.element_free))free;         \
	II__make_room(array, count);                                   \
	for (size_t i = 0; i < count; i++)                             \
		array.elements[i] = parameters[i];                         \
} while(0)

#define II__free(array)                                \
do {                                                   \
	if (array.elements)                                \
	{                                                  \
		if (array.element_free)                        \
			for (size_t i = 0; i < array.length; i++)  \
				array.element_free(array.elements[i]); \
		free(array.elements);                          \
		array.elements = NULL;                         \
	}                                                  \
} while(0)

#define II__concat(original, ...)                                       \
do {                                                                    \
	typeof(*original.elements) parameters[] = {__VA_ARGS__};            \
	size_t count = sizeof(parameters)/sizeof(typeof(*parameters));      \
	const size_t total = original.length + count;                       \
                                                                        \
	if (original.capacity < total)                                      \
		II__move_room(original, total);                                 \
	for (size_t i = 0; original.length < total; original.length++, i++) \
		original.elements[original.length] = parameters[i];             \
} while(0)

#define II__append(array, element)         \
do {                                       \
	const size_t last = array.length++;    \
                                           \
	if (array.capacity < array.length)     \
	{                                      \
		II__move_room(array, array.length) \
		array.elements[last] = element;    \
	}                                      \
	else                                   \
		array.elements[last] = element;    \
} while(0)

#define II__remove_last(array)                      \
	array.element_free(array.elements[--array.length]) 

#define II__duplicate(copy, original)                                                       \
{                                                                                           \
	copy = original;                                                                        \
	copy.elements = malloc(copy.capacity);                                                  \
	memcpy(copy.elements, original.elements, sizeof(typeof(*copy.elements)) * copy.length); \
}
