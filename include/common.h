#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

/*
- BRIEF:        immediately terminate process with a message
- PARAM:        message to print
- RET:          void
- NOTE:         calls exit(EXIT_FAILURE)
*/
void            die(const char *msg) __attribute__((noreturn));

/*
- BRIEF:        allocate memory
- PARAM:        bytes to allocate
- RET:          pointer to new memory
- NOTE:         safe wrapper around malloc(...); upon failure, calls die(...)
*/
void            *mem_alloc(size_t size);

/*
- BRIEF:        free memory
- PARAM:        pointer to free
- RET:          void
- NOTE:         wrapper around free(...)
*/
void            mem_free(void *ptr);

#endif

