#include "array.h"
#include <stdio.h>

typedef const char* string;
declare_array(string);

void cleanup(string element)
{
	printf("I'd have totally freed this [%s]\n", element);
}

int main()
{
	IstringI words;

	II__init(words, cleanup, "hello", "world");
	II__concat(words, "and", "bye", "word");
	II__remove_last(words);
	II__append(words, "world");
	
	putchar('\n');
	for (size_t i = 0; i < words.length; i++)
		printf("%s ", words.elements[i]);
	puts("\n");

	II__free(words);
}
