#ifndef PARSER_H
#define PARSER_H

#include "../include/lexer.h"

typedef enum {
	AST_NUM,
	AST_NEG,
	AST_ADD,
	AST_SUB,
	AST_MUL,
	AST_DIV,
	AST_POW,
	AST_PRINT,
	AST_LET,
	AST_IDENT,
	AST_PROGRAM
} ast_node_type_t;

typedef		struct _ast_node	ast_node_t;

/*
- BRIEF:	parse the program
- PARAM:	lexer
- RET:		ast program node (root)
- NOTE:		calls lexer for every token
*/
ast_node_t	*parse_program(lexer_t *lexer);

/*
- BRIEF:	evaluate ast node
- PARAM:	node
- RET:		void
*/
void		ast_eval(ast_node_t *node, FILE *stream);

/*
- BRIEF:	dump all ast nodes to a stream
- PARAM:	node
- RET:		void
*/
void		ast_dump(ast_node_t *node, FILE *stream);

#endif

