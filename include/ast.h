#ifndef AST_H
#define AST_H

#include "../include/common.h"

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

typedef	struct _ast_node ast_node_t;

ast_node_t *ast_node_new(ast_node_type_t type, value_t value, ast_node_t *left, ast_node_t *right);
// void ast_free(ast_t tree);
const char *ast_node_type_str(ast_node_type_t type);

void ast_dump(ast_t tree, FILE *stream);

void ast_eval(ast_t tree, symbol_table_t *table, FILE *stream);

#endif

