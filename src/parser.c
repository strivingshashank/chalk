#include "../include/common.h"
#include "../include/parser.h"

#include <stdio.h>
#include <math.h>

/* ----- DECLARATIONS ----- */

struct _ast_node {
	ast_node_type_t type;
	double          value;
	ast_node_t      *left;
	ast_node_t      *right;
};

static ast_node_t *_parse_statement(lexer_t *lexer);
static ast_node_t *_parse_print(lexer_t *lexer);

static ast_node_t *_parse_expression(lexer_t *lexer);
static ast_node_t *_parse_term(lexer_t *lexer);
static ast_node_t *_parse_power(lexer_t *lexer);
static ast_node_t *_parse_unary(lexer_t *lexer);
static ast_node_t *_parse_primary(lexer_t *lexer);

static void _ast_dump_with_depth(ast_node_t *node, int depth, FILE *stream);

static ast_node_t *_ast_node_new(ast_node_type_t type, double value, ast_node_t *left, ast_node_t *right);

/* ----- DEFINITIONS ----- */

ast_node_t *_ast_node_new(ast_node_type_t type, double value, ast_node_t *left, ast_node_t *right) {
	ast_node_t *node = mem_alloc(sizeof(ast_node_t));
	
	node->type = type;
	node->value = value;
	node->left = left;
	node->right = right;
	
	return node;
}

ast_node_t *parse_program(lexer_t *lexer) {
	if (!lexer) {
		die("parse_program(): null lexer");
	}

	if (lexer_peek(lexer).type == TOK_EOF) {
		return NULL;
	}

	ast_node_t *stmt = _parse_statement(lexer);
	ast_node_t *rest = parse_program(lexer);

	return _ast_node_new(AST_PROGRAM, 0, stmt, rest);
}

// TODO: add more statements (let var/func, ...)
ast_node_t *_parse_statement(lexer_t *lexer) {
	ast_node_t *left = _parse_print(lexer);
	
	token_t token = lexer_peek(lexer);

	if (token.type != TOK_SCOLON) {
		die("_parse_statement(): expected a ;");
	}

	lexer_next(lexer);
	
	return left;
}

ast_node_t *_parse_print(lexer_t *lexer) {
	token_t token = lexer_peek(lexer);

	if (token.type != TOK_PRINT) {
		die("_parse_print(): expected print keyword");
	}

	token = lexer_next(lexer);
	token = lexer_peek(lexer);

	if (token.type != TOK_COLON) {
		printf("%d\n", token.type);
		die ("_parse_print(): expected a :");
	}

	token = lexer_next(lexer);

	ast_node_t *left = _parse_expression(lexer);

	return _ast_node_new(AST_PRINT, 0, left, NULL);
}

ast_node_t *_parse_expression(lexer_t *lexer) {
	ast_node_t *left = _parse_term(lexer);
	
	for (;;) {
		token_t token = lexer_peek(lexer);
		ast_node_type_t node_type;
		
		if (token.type == TOK_ADD) {
			node_type = AST_ADD;
		} else if (token.type == TOK_SUB) {
			node_type = AST_SUB;
		} else {
			break;
		}
		
		lexer_next(lexer);
		ast_node_t *right = _parse_term(lexer);
		left = _ast_node_new(node_type, 0, left, right);
	}
	
	return left;
}

ast_node_t *_parse_term(lexer_t *lexer) {
	ast_node_t *left = _parse_power(lexer);
	
	for (;;) {
		token_t token = lexer_peek(lexer);
		ast_node_type_t node_type;
		
		if (token.type == TOK_MUL) {
			node_type = AST_MUL;
		} else if (token.type == TOK_DIV) {
			node_type = AST_DIV;
		} else {
			break;
		}
		
		lexer_next(lexer);
		ast_node_t *right = _parse_power(lexer);
		left = _ast_node_new(node_type, 0, left, right);
	}
	
	return left;
}

ast_node_t *_parse_power(lexer_t *lexer) {
	ast_node_t *left = _parse_unary(lexer);

	token_t token = lexer_peek(lexer);

	if (token.type == TOK_POW) {
		lexer_next(lexer);
		ast_node_t *right = _parse_power(lexer);
		return _ast_node_new(AST_POW, 0, left, right);
	}

	return left;
}

ast_node_t *_parse_unary(lexer_t *lexer) {
	token_t token = lexer_peek(lexer);
	
	if (token.type == TOK_SUB) {
		lexer_next(lexer);
		ast_node_t *operand = _parse_unary(lexer);
		return _ast_node_new(AST_NEG, 0, operand, NULL);
	}
	
	return _parse_primary(lexer);
}

ast_node_t *_parse_primary(lexer_t *lexer) {
	token_t token = lexer_peek(lexer);
	
	if (token.type == TOK_NUM) {
		lexer_next(lexer);
		return _ast_node_new(AST_NUM, token.value, NULL, NULL);
	}  
	
	if (token.type != TOK_LPAREN) {
		die("_parse_primary(): expected a (");
	}
	
	lexer_next(lexer); /* consume '(' */
	ast_node_t *expression = _parse_expression(lexer);
	token = lexer_next(lexer); /* consume ')' */
	
	if (token.type != TOK_RPAREN) {
		printf("token_type: %d\n", token.type);
		die("_parse_primary(): expected a )");
	}
	
	return expression;
}

static const char *ast_node_type_str(ast_node_type_t type) {
	switch (type) {
		case AST_NUM:		return "NUM";
		case AST_NEG:		return "NEG";
		case AST_ADD:		return "ADD";
		case AST_SUB:		return "SUB";
		case AST_MUL:		return "MUL";
		case AST_DIV:		return "DIV";
		case AST_POW:		return "POW";
		case AST_PRINT:		return "PRINT";
		case AST_PROGRAM:	return "PROGRAM";
		default:      		return "UNKNOWN";
	}
}

double ast_eval_value(ast_node_t *node) {
	if (!node) {
		die("ast_eval_value(): null node");
	}

	switch (node->type) {
		case AST_NUM: return node->value;
		case AST_NEG: return -ast_eval_value(node->left);
		case AST_ADD: return ast_eval_value(node->left) + ast_eval_value(node->right);
		case AST_SUB: return ast_eval_value(node->left) - ast_eval_value(node->right);
		case AST_MUL: return ast_eval_value(node->left) * ast_eval_value(node->right);
		case AST_DIV: return ast_eval_value(node->left) / ast_eval_value(node->right);
		case AST_POW: return pow(ast_eval_value(node->left), ast_eval_value(node->right));
		default:	die("ast_eval_value(): unexpected AST node");
	}
}

void ast_eval(ast_node_t *node, FILE *stream) {
	if (!node) {
		return;
	}

	switch (node->type) {
		case AST_PROGRAM: {
			ast_eval(node->left, stream);
			ast_eval(node->right, stream);
			break;
		}

		case AST_PRINT: {
			fprintf(stream, "%g\n", ast_eval_value(node->left));
			break;
		}

		default: {
			die("ast_eval(): invalid ast node");
		}
	}
}

void ast_dump(ast_node_t *node, FILE *stream) {
	if (!node) {
		die("ast_dump(): null node");
	}

	if (!stream) {
		die("ast_dump(): null stream");
	}

	_ast_dump_with_depth(node, 0, stream);
}

void _ast_dump_with_depth(ast_node_t *node, int depth, FILE *stream) {
	if (!node) {
		return;
	}
	
	for (int indent = 0; indent < depth; indent++) {
		fprintf(stream, " ");
	}
	
	if (node->type == AST_NUM) {
		fprintf(stream, "NUM(%g)\n", node->value);
	} else {
		fprintf(stream, "%s\n", ast_node_type_str(node->type));
	}
	
	_ast_dump_with_depth(node->left, depth + 1, stream);
	_ast_dump_with_depth(node->right, depth + 1, stream);
}

