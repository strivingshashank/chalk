#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

/*
- BRIEF:  immediately terminates with exit(1) call and prints a message
- PARAM:  message to print
- RET:    void
*/
void  die(const char *msg) __attribute__((noreturn));

/*
- BRIEF:  safe wrapper for malloc(...)
- PARAM:  bytes to allocate
- RET:    void pointer to new memory
- NOTE:   upon failure, calls die(...)
*/
void *mem_alloc(size_t size);

/*
- BRIEF:  wrapper for free(...)
- PARAM:  pointer to free
- RET:    void
*/
void  mem_free(void *ptr);

#endif

