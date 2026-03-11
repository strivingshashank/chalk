#include "../include/common.h"
#include "../include/lexer.h"
#include "../include/parser.h"

#include <stdio.h>

int main() {
	const char expression[] = "4*2/1-5";
	// const char expression[] = "-(3+4*2)/(1-5)";
	lexer_t *lexer = lexer_new(expression);
	// lexer_dump(lexer);
	
	ast_node_t *tree = parse_expression(lexer);
	
	printf("Expression: %s\n", expression);
	printf("Result: %g\n", ast_eval(tree));
	printf("AST:\n");
	ast_dump(tree, 0);
	
	return 0;
}

