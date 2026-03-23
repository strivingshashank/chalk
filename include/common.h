/**
 * @file common.h
 * @brief Shared utilities.
 */

#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

/** @brief The file extension for Chalk source files. */
#define FILE_EXTENSION          ".chalk"

/** @brief Length of FILE_EXTENSION, excluding the null terminator. */
#define FILE_EXTENSION_LEN      (sizeof(FILE_EXTENSION) - 1)


/**
 * @brief Immediately terminate the process with an error message.
 *
 * Prints @p msg to stderr prefixed with "Error: " and calls exit(EXIT_FAILURE).
 * Marked noreturn - the compiler knows control never returns from this call.
 *
 * @param msg Message to print before exiting.
 */
void die(const char *msg) __attribute__((noreturn));

/**
 * @brief Allocate memory, dying on failure.
 *
 * Thin wrapper around malloc(). If allocation fails, calls die() rather than
 * returning NULL - callers never need to check the return value.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to allocated memory. Never NULL.
 */
void *mem_alloc(size_t size);


/**
 * @brief Free allocated memory.
 *
 * Wrapper around free(). Passing NULL is safe.
 *
 * @param ptr Pointer to free.
 */
void mem_free(void *ptr);

#endif

