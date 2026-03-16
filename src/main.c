#include "../include/common.h"
#include "../include/lexer.h"
#include "../include/parser.h"

#include <stdio.h>

const char *get_expression(void) {
	return "3^2^2";
	// return "1.1+2.2";
	// return "4*2/1-5";
	// return "-(3+4*2)/(1-5)";
	// return "((15+7)*(8-3)-(42/6+11)*(3+4))";
	// return "(100/(4+1))*(7-2)+(36/((2+1)*4)-1)";
	// return "((84/(2+5))*(9-4)-((13+7)/(2+3)))*(4-1)";
}

int main() {
	const char *expression = get_expression();
	lexer_t *lexer = lexer_new(expression);
	ast_node_t *tree = parse_expression(lexer);
	
	printf("----- Expression -----\n");
	printf("%s\n", expression);
	
	printf("----- Result -----\n");
	printf("%g\n", ast_eval(tree));
	
	printf("----- AST -----\n");
	ast_dump(tree, 0);
	
	return 0;
}

