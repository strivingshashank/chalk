#ifndef PARSER_H
#define PARSER_H

#include "../include/lexer.h"

typedef enum {
	AST_NUM,
	AST_NEG,
	AST_ADD,
	AST_SUB,
	AST_MUL,
	AST_DIV
} ast_node_type_t;

typedef struct ast_node ast_node_t;

ast_node_t *parse_expression(lexer_t *lexer);

double      ast_eval(ast_node_t *node);
void        ast_dump(ast_node_t *node, int depth);

#endif

