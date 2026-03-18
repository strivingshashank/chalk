#include "../include/common.h"
#include "../include/lexer.h"
#include "../include/parser.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ----- DECLARATIONS ----- */
char *get_source_from_file(const char *file_name);

// TODO: create readme, better error reporting
int main() {
	FILE *out_lexer = fopen("tests/output_lexer.txt", "w");
	FILE *out_parser = fopen("tests/output_parser.txt", "w");
	FILE *out_program = fopen("tests/output_program.txt", "w");
	
	if (!out_lexer) {
		die("main(): null out_lexer");
	}
	
	if (!out_parser) {
		die("main(): null out_parser");
	}
	
	if (!out_program) {
		die("main(): null out_program");
	}
	
	char *source_program = get_source_from_file("example/mix.chalk");
	lexer_t *lexer = lexer_new(source_program);
	ast_node_t *program_tree = parse_program(lexer);

	printf("----- Program -----\n");
	printf("%s\n", source_program);

	printf("----- Output -----\n");
	ast_eval(program_tree, stdout);
	
	// printf("----- AST -----\n");
	// ast_dump(program_tree, stdout);

	/* clean-up */
	fclose(out_lexer);
	fclose(out_parser);
	fclose(out_program);
	free(source_program);
	lexer_free(&lexer);
	
	return 0;
}

/* ----- DEFINITIONS ----- */

char *get_source_from_file(const char *file_name) {
	if (!file_name) {
		die("get_source_from_file(): null file name");
	}

	FILE *file = fopen(file_name, "r");

	if (!file) {
		die("get_source_from_file(): cannot open file");
	}
	
	fseek(file, 0, SEEK_END); /* set file cursor at EOF */
	long file_size = ftell(file);
	rewind(file);

	/* one extra byte accounts for the NULL terminator */
	char *buff = mem_alloc(sizeof(char) * file_size + 1);
	fread(buff, sizeof(char), file_size, file);
	buff[file_size] = '\0';

	fclose(file);

	return buff;
}

