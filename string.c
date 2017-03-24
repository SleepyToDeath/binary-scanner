#include "ac/ac.h"
#include <stdio.h>

void found_callback(struct task* t, unsigned long index, unsigned long offset)
{

}

void* next_block_callback(struct task* t);
{

}

int main(int argc, char* argv[])
{
	FILE* patternf;
	FILE* mainf;
	patternf = fopen("pattern.txt");
	mainf = fopen("main.txt");
	struct task* t;
	t = malloc(sizeof(struct task));
	t->malloc = malloc;
	t->free = free;
	t->found_callback = found_callback;
	t->next_block_callback = next_block_callback;
}
