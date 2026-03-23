#include "../include/common.h"
#include "../include/lexer.h"
#include "../include/parser.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(x)	((void)x)

/* ----- DECLARATIONS ----- */
char *get_source_from_file(const char *file_name);

// TODO: better error reporting
// TODO: symbol table implementation
// TODO: use asserts in internal functions
// TODO: write doxy comments for internal fn

int main(int argc, char **argv) {
	if (argc != 2) {
		die("main(): usage ./chalk file_name.chalk");
	}
	
	char *source_program = get_source_from_file(argv[1]);
	lexer_t *lexer = lexer_new(source_program);
	ast_node_t *program_tree = parse_program(lexer);

	// UNUSED(program_tree);

	printf("----- Program -----\n");
	printf("%s\n", source_program);

	// printf("----- Lexer Dump -----\n");
	// lexer_dump(lexer, stdout);
	
	// printf("----- Output -----\n");
	// ast_eval(program_tree, stdout);
	
	printf("----- AST -----\n");
	ast_dump(program_tree, stdout);

	/* clean-up */
	free(source_program);
	lexer_free(&lexer);
	
	return 0;
}

/* ----- DEFINITIONS ----- */

// TODO: can I improve file checking here?
char *get_source_from_file(const char *file_name) {
	if (!file_name) {
		die("get_source_from_file(): null file name");
	}
	
	char *file_extension = strstr(file_name, FILE_EXTENSION);
	
	if (!file_extension) {
		die("get_source_from_file(): invalid extension, use \".chalk\"");
	}

	if (strlen(file_extension) != FILE_EXTENSION_LEN) {
		die("get_source_from_file(): invalid extension, use \".chalk\"");
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

