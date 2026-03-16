#include "../include/common.h"
#include "../include/parser.h"

#include <stdio.h>
#include <math.h>

struct ast_node {
	ast_node_type_t type;
	double          value;
	ast_node_t      *left;
	ast_node_t      *right;
};

/* ----- FORWARD DECLARATIONS ----- */
ast_node_t *parse_term(lexer_t *lexer);
ast_node_t *parse_power(lexer_t *lexer);
ast_node_t *parse_unary(lexer_t *lexer);
ast_node_t *parse_primary(lexer_t *lexer);

static ast_node_t *ast_node_new(ast_node_type_t type, double value, ast_node_t *left, ast_node_t *right) {
	ast_node_t *node = mem_alloc(sizeof(ast_node_t));
	
	node->type = type;
	node->value = value;
	node->left = left;
	node->right = right;
	
	return node;
}

ast_node_t *parse_expression(lexer_t *lexer) {
	if (!lexer) {
		die("parse_expression(): null lexer");
	}
	
	ast_node_t *left = parse_term(lexer);
	
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
		ast_node_t *right = parse_term(lexer);
		left = ast_node_new(node_type, 0, left, right);
	}
	
	return left;
}

ast_node_t *parse_term(lexer_t *lexer) {
	ast_node_t *left = parse_power(lexer);
	
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
		ast_node_t *right = parse_power(lexer);
		left = ast_node_new(node_type, 0, left, right);
	}
	
	return left;
}

ast_node_t *parse_power(lexer_t *lexer) {
	ast_node_t *left = parse_unary(lexer);

	token_t token = lexer_peek(lexer);

	if (token.type == TOK_POW) {
		lexer_next(lexer);
		ast_node_t *right = parse_power(lexer);
		return ast_node_new(AST_POW, 0, left, right);
	}

	return left;
}

ast_node_t *parse_unary(lexer_t *lexer) {
	token_t token = lexer_peek(lexer);
	
	if (token.type == TOK_SUB) {
		lexer_next(lexer);
		ast_node_t *operand = parse_unary(lexer);
		return ast_node_new(AST_NEG, 0, operand, NULL);
	}
	
	return parse_primary(lexer);
}

ast_node_t *parse_primary(lexer_t *lexer) {
	token_t token = lexer_peek(lexer);
	
	if (token.type == TOK_NUM) {
		lexer_next(lexer);
		return ast_node_new(AST_NUM, token.value, NULL, NULL);
	}  
	
	if (token.type != TOK_LPAREN) {
		die("parse_primary(): expected TOK_LPAREN");
	}
	
	lexer_next(lexer); /* consume '(' */
	ast_node_t *expression = parse_expression(lexer);
	token = lexer_next(lexer); /* consume ')' */
	
	if (token.type != TOK_RPAREN) {
		printf("token_type: %d\n", token.type);
		die("parse_primary(): expected TOK_RPAREN");
	}
	
	return expression;
}

static const char *ast_node_type_str(ast_node_type_t type) {
	switch (type) {
		case AST_NUM: return "NUM";
		case AST_NEG: return "NEG";
		case AST_ADD: return "ADD";
		case AST_SUB: return "SUB";
		case AST_MUL: return "MUL";
		case AST_DIV: return "DIV";
		case AST_POW: return "POW";
		default:      return "UNKNOWN";
	}
}

double ast_eval(ast_node_t *node) {
	if (!node) {
		die("ast_eval(): null node");
	}
	
	switch (node->type) {
		case AST_NUM: return node->value;
		case AST_NEG: return -ast_eval(node->left);
		case AST_ADD: return ast_eval(node->left) + ast_eval(node->right);
		case AST_SUB: return ast_eval(node->left) - ast_eval(node->right);
		case AST_MUL: return ast_eval(node->left) * ast_eval(node->right);
		case AST_DIV: return ast_eval(node->left) / ast_eval(node->right);
		case AST_POW: return pow(ast_eval(node->left), ast_eval(node->right));
		default:	die("ast_eval(): unexpected AST node");
	}

	return 0;
}

void ast_dump(ast_node_t *node, int depth) {
	if (!node) {
		return;
	}
	
	for (int indent = 0; indent < depth; indent++) {
		printf(" ");
	}
	
	if (node->type == AST_NUM) {
		printf("NUM(%g)\n", node->value);
	} else {
		printf("%s\n", ast_node_type_str(node->type));
	}
	
	ast_dump(node->left, depth + 1);
	ast_dump(node->right, depth + 1);
}

