#include "../include/common.h"
#include "../include/lexer.h"
#include "../include/parser.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_expression_from_file(const char *file_name);

int main() {
	const char *expression = get_expression_from_file("example/mix.chalk");
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

char *get_expression_from_file(const char *file_name) {
	if (!file_name) {
		die("get_expression_from_file(): null file name");
	}

	FILE *file = fopen(file_name, "r");

	if (!file) {
		die("get_expression_from_file(): cannot open file");
	}
	
	fseek(file, 0, SEEK_END); /* set file cursor at EOF */
	long file_size = ftell(file);
	rewind(file);

	/* one extra byte accounts for the NULL terminator */
	char *buff = mem_alloc(sizeof(char) * file_size + 1);
	fread(buff, sizeof(char), file_size, file);
	buff[file_size] = '\0';

	return buff;
}

