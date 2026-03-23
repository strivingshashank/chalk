/**
 * @file lexer.h
 * @brief Lexer API: tokenisation of Chalk source text.
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

/** @brief Maximum number of characters in an identifier. */
#define MAX_IDENT_LEN 32

/**
 * @brief All token types produced by the lexer.
 *
 * TOK_UNKNOWN is returned for any lexeme the lexer does not recognise.
 * TOK_EOF signals end of input.
 */
typedef enum {
	TOK_NUM,      /**< Numeric literal (integer or float). */
	TOK_ADD,      /**< '+' operator. */
	TOK_SUB,      /**< '-' operator. */
	TOK_MUL,      /**< '*' operator. */
	TOK_DIV,      /**< '/' operator. */
	TOK_POW,      /**< '^' operator. */
	TOK_LPAREN,   /**< '(' operator. */
	TOK_RPAREN,   /**< ')' operator. */
	TOK_PRINT,    /**< 'print' keyword. */
	TOK_COLON,    /**< ':' separator. */
	TOK_SCOLON,   /**< ';' statement terminator. */
	TOK_LET,      /**< 'let' keyword. */
	TOK_IDENT,    /**< Identifier. */
	TOK_EOF,      /**< End of source input. */
	TOK_UNKNOWN   /**< Unrecognised character. */
} token_type_t; 

/**
 * @brief A single token produced by the lexer.
 *
 * The active union member depends on type:
 * - TOK_NUM   -> value.num
 * - TOK_IDENT -> value.str
 * - All other -> token type is enough, no value needed.
 */
typedef struct {
	token_type_t type; /**< Token type, decides union value. */
	
	union {
		double num;  /**< Number value (for TOK_NUM only). */
		char str[MAX_IDENT_LEN + 1]; /**< Identifier name (for TOK_IDENT only). */
	} value;
} token_t;


/**
 * @brief Opaque lexer handle.
 *
 * Internal layout is defined in lexer.c. Callers interact only via the
 * lexer_*() API - never access the fields directly.
 */
typedef struct _lexer lexer_t;

/**
 * @brief Create a new lexer for the given source string.
 *
 * The lexer does not copy @p src - the caller must keep it alive for
 * the lexer's lifetime.
 *
 * @param src Null-terminated source program. Must not be NULL.
 * @return Newly allocated lexer. Free with lexer_free().
 */
lexer_t *lexer_new(const char *src);

/**
 * @brief Free a lexer and set its pointer to NULL.
 *
 * Safe to call with a NULL @p lexer_addr or a NULL @p *lexer_addr.
 *
 * @param lexer_addr Address of the lexer pointer to free and nullify.
 */
void lexer_free(lexer_t **lexer_addr);

/**
 * @brief Consume and return the next token.
 *
 * If a peeked token is cached, returns and clears it without re-scanning.
 *
 * @param lexer Active lexer. Must not be NULL.
 * @return Next token in the stream.
 */
token_t lexer_next(lexer_t *lexer);

/**
 * @brief Return the next token without consuming it.
 *
 * Subsequent calls to lexer_peek() return the same token until
 * lexer_next() is called.
 *
 * @param lexer Active lexer. Must not be NULL.
 * @return Next token in the stream (same as the next lexer_next() call).
 */
token_t lexer_peek(lexer_t *lexer);

/**
 * @brief Dump the full token stream to @p stream.
 *
 * Consumes all remaining tokens. Calls die() on TOK_UNKNOWN.
 *
 * @param lexer Active lexer. Must not be NULL.
 * @param stream Output stream. Must not be NULL.
 */
void lexer_dump(lexer_t *lexer, FILE *stream);

#endif

