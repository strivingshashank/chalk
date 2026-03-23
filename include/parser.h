/**
 * @file parser.h
 * @brief Parser API: recursive descent parser, AST definition, evaluator.
 */


#ifndef PARSER_H
#define PARSER_H

#include "../include/lexer.h"

/**
 * @brief AST node types.
 *
 * Controls how ast_eval() and ast_dump() interpret a node and its children.
 */
typedef enum {
	AST_NUM,     /**< Numeric literal leaf. */
	AST_NEG,     /**< Unary negation. */
	AST_ADD,     /**< Binary addition. */
	AST_SUB,     /**< Binary subtraction. */
	AST_MUL,     /**< Binary multiplication. */
	AST_DIV,     /**< Binary division. */
	AST_POW,     /**< Exponentiation. */
	AST_PRINT,   /**< Print statement. */
	AST_LET,     /**< Let statement. */
	AST_IDENT,   /**< Identifier. */
	AST_PROGRAM  /**< Program node - chains statements. */
} ast_node_type_t;

/**
 * @brief Opaque AST node.
 *
 * Internal layout defined in parser.c. All access is through
 * parse_program(), ast_eval(), and ast_dump().
 */
typedef	struct _ast_node ast_node_t;

/**
 * @brief Parse a complete Chalk program into an AST.
 *
 * Repeatedly calls _parse_statement() until TOK_EOF. Each call returns
 * an AST_PROGRAM node whose left child is the parsed statement and whose
 * right child is the rest of the program (or NULL at end of input).
 *
 * @param lexer Active lexer positioned at the start of input. Must not be NULL.
 * @return Root AST_PROGRAM node, or NULL if the input is empty.
 */
void *parse_program(lexer_t *lexer);

/**
 * @brief Evaluate an AST and write output to @p stream.
 *
 * Walks the tree recursively and acts based on the active node.
 *
 * @param node Root node to evaluate.
 * @param stream Output stream for print statements.
 */
void ast_eval(ast_node_t *node, FILE *stream);


/**
 * @brief Dump a human-readable representation of the AST to @p stream.
 *
 * Indentation reflects tree depth (1 space per level).
 *
 * @param node Root node. Must not be NULL.
 * @param stream Output stream. Must not be NULL.
 */
void ast_dump(ast_node_t *node, FILE *stream);

#endif

