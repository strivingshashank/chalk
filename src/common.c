#include "../include/common.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void die(const char *msg) {
	fprintf(stderr, "Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

void *mem_alloc(size_t size) {
	void *ptr = malloc(size);
	
	if (!ptr) {
		die("mem_alloc(): malloc failed");
	}
	
	return ptr;
}

void mem_free(void *ptr) {
	free(ptr);
}

