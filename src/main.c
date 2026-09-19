#include "../include/vec.h"

#include "../include/common.h"
#include "../include/lexer.h"
#include "../include/symbol_table.h"
#include "../include/parser.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(x)	((void)x)

/* ----- DECLARATIONS ----- */
typedef vec_t(token_t) vec_tok_t;

char *get_source_from_file(const char *file_name);

// TODO: better error reporting
// TODO: symbol table implementation (almost done, add free fn too)
// TODO: use asserts in internal functions
// TODO: write doxy comments for internal fn
// TODO: refactor code
// TODO: add cli args for printing ast, lexer dump and all (instead of hard-coding source)

char *tok_to_str(token_t tok) {
	switch (tok.type) {
		case TOK_EOF: {
			return "TOK_EOF";
		}
		
		case TOK_UNKNOWN: {
			return "TOK_UNKNOWN";
		}
		
		case TOK_NUM: {
			return "TOK_NUM";
		}
		
		case TOK_ADD: {
			return "TOK_ADD";
		}
		
		case TOK_SUB: {
			return "TOK_SUB";
		}
		
		case TOK_MUL: {
			return "TOK_MUL";
		}
		
		case TOK_DIV: {
			return "TOK_DIV";
		}
		
		case TOK_POW: {
			return "TOK_POW";
		}
		
		case TOK_LPAREN: {
			return "TOK_LPAREN";
		}
		
		case TOK_RPAREN: {
			return "TOK_RPAREN";
		}
		
		case TOK_PRINT: {
			return "TOK_PRINT";
		}

		case TOK_LET: {
			return "TOK_LET";
		}

		case TOK_COLON: {
			return "TOK_COLON";
		}

		case TOK_SCOLON: {
			return "TOK_SCOLON";
		}


		case TOK_IDENT: {
			return "TOK_IDENT";
		}

		default: {
			die("lexer_dump(): unknown token");
		}
	}
}

int main(int argc, char **argv) {
	if (argc != 2) {
		die("main(): usage ./chalk file_name.chalk");
	}
	
	char *source_program = get_source_from_file(argv[1]);
	lexer_t *lexer = lexer_new(source_program);
	
	ast_node_t *program_tree = parse_program(lexer);
	symbol_table_t *table = symbol_table_new(symbol_count(program_tree));

	// UNUSED(table);
	// // UNUSED(program_tree);

	// printf("----- Program -----\n");
	// printf("%s\n", source_program);


	// vec_tok_t tokens;
	// vec_init(&tokens);

	// token_t tok = lexer_peek(lexer);

	// while (tok.type != TOK_EOF) {
	// 	tok = lexer_next(lexer);
	// 	vec_push(&tokens, tok);
	// }

	// for (int idx = 0; idx < tokens.length; idx++) {
	// 	printf("%s\n", tok_to_str(tokens.data[idx]));
	// }
	
	// printf("----- Symbol Count -----\n");
	// printf("%ld symbols\n", symbol_count(program_tree));

	// printf("----- Lexer Dump -----\n");
	// lexer_dump(lexer, stdout);
		
	// printf("----- AST -----\n");
	// ast_dump(program_tree, stdout);
	
	printf("----- Output -----\n");
	ast_eval(program_tree, table, stdout);

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

