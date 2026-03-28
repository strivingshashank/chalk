#include "../include/lexer.h"
#include "../include/common.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>

#define KW_PRINT "print"
#define KW_PRINT_LEN (sizeof("print") - 1)

#define KW_LET "let"
#define KW_LET_LEN (sizeof("let") - 1)

/* ----- DECLARATIONS ----- */

struct _lexer {
	const char *src;
	size_t      pos;
	token_t     peeked;
	bool        has_peeked;
};

static token_t _token_next(lexer_t *lexer);
static token_t _token_form_number(lexer_t *lexer);
static token_t _token_form_separator(lexer_t *lexer);
static token_t _token_form_keyword(lexer_t *lexer);
static token_t _token_form_ident(lexer_t *lexer);

/* ----- DEFINITIONS ----- */

token_t _token_next(lexer_t *lexer) {
	assert(lexer);
	
	/* skip whitespace */
	while (isspace(lexer->src[lexer->pos])) {
		lexer->pos++;
	}
	
	/* handle NULL */
	if (lexer->src[lexer->pos] == '\0') {
		return (token_t){ .type = TOK_EOF, .value.num = 0 };
	}
	
	/* form number */
	if (isdigit(lexer->src[lexer->pos])) {
		return _token_form_number(lexer);
	}

	/* form keywords/identifiers */
	if (isalpha(lexer->src[lexer->pos])) {
		token_t token = _token_form_keyword(lexer);

		if (token.type != TOK_UNKNOWN) {
			return token;
		}

		return _token_form_ident(lexer);
	}
	
	return _token_form_separator(lexer);
}

token_t _token_form_number(lexer_t *lexer) {
	assert(lexer);

	/* interger part */
	double value = 0;
	
	while (isdigit(lexer->src[lexer->pos])) {
		value = value * 10 + (lexer->src[lexer->pos] - '0');
		lexer->pos++;
	}
	
	/* fraction part */
	if (lexer->src[lexer->pos] == '.') {
		lexer->pos++;
		double factor = 0.1;
		
		while (isdigit(lexer->src[lexer->pos])) {
			value += (lexer->src[lexer->pos] - '0') * factor;
			factor *= 0.1;
			lexer->pos++;
		}
	}
	
	return (token_t){ .type = TOK_NUM, .value.num = value };
}

token_t _token_form_separator(lexer_t *lexer) {
	assert(lexer);

	/* increment the cursor here beacause, we do not get the chance later */
	lexer->pos++;

	switch (lexer->src[lexer->pos - 1]) {
		case '\0': return (token_t){ .type = TOK_EOF, .value.num = 0 };
		case '+': return (token_t){ .type = TOK_ADD, .value.num = 0 };
		case '-': return (token_t){ .type = TOK_SUB, .value.num = 0 };
		case '*': return (token_t){ .type = TOK_MUL, .value.num = 0 };
		case '/': return (token_t){ .type = TOK_DIV, .value.num = 0 };
		case '^': return (token_t){ .type = TOK_POW, .value.num = 0 };
		case '(': return (token_t){ .type = TOK_LPAREN, .value.num = 0 };
		case ')': return (token_t){ .type = TOK_RPAREN, .value.num = 0 };
		case ':': return (token_t){ .type = TOK_COLON, .value.num = 0 };
		case ';': return (token_t){ .type = TOK_SCOLON, .value.num = 0 };
		default:  return (token_t){ .type = TOK_UNKNOWN, .value.num = 0 };
	}
}

static token_t _token_form_keyword(lexer_t *lexer) {
	assert(lexer);

	if (strncmp(&lexer->src[lexer->pos], KW_PRINT, KW_PRINT_LEN) == 0) {
		if (isalnum(lexer->src[lexer->pos + KW_PRINT_LEN])) {
			return (token_t){ .type = TOK_UNKNOWN, .value.num = 0 };
		}

		lexer->pos += KW_PRINT_LEN;
		return (token_t){ .type = TOK_PRINT, .value.num = 0 };
	}

	if (strncmp(&lexer->src[lexer->pos], KW_LET, KW_LET_LEN) == 0) {
		if (isalnum(lexer->src[lexer->pos + KW_LET_LEN])) {
			return (token_t){ .type = TOK_UNKNOWN, .value.num = 0 };
		}

		lexer->pos += KW_LET_LEN;
		return (token_t){ .type = TOK_LET, .value.num = 0 };
	}

	return (token_t){ .type = TOK_UNKNOWN, .value.num = 0 };
}

token_t _token_form_ident(lexer_t *lexer) {
	assert(lexer);

	token_t token;
	long ident_len = 0;

	while (isalpha(lexer->src[lexer->pos + ident_len])) {
		ident_len++;
	}

	if (MAX_IDENT_LEN < ident_len) {
		die("_token_form_ident(): max ident len reached");
	}

	token.type = TOK_IDENT;
	strncpy(token.value.str, &lexer->src[lexer->pos], ident_len);
	token.value.str[ident_len] = '\0';
	
	lexer->pos += ident_len;
	
	return token;
}

lexer_t *lexer_new(const char *src) {
	if (!src) {
		die("lexer_new(): null source");
	}
	
	lexer_t *lexer = mem_alloc(sizeof(lexer_t));
	
	lexer->src = src;
	lexer->pos = 0;
	lexer->has_peeked = false;
	
	return lexer;
}

void lexer_free(lexer_t **lexer_addr) {
	if (!lexer_addr || !*lexer_addr) {
		return;
	}
	
	free(*lexer_addr);
	*lexer_addr = NULL;
}

token_t lexer_next(lexer_t *lexer) {
	if (!lexer) {
		die("lexer_next(): null lexer");
	}
	
	if (lexer->has_peeked) {
		lexer->has_peeked = false;
		return lexer->peeked;
	}
	
	return _token_next(lexer);
}

token_t lexer_peek(lexer_t *lexer) {
	if (!lexer) {
		die("lexer_peek(): null lexer");
	}
	
	if (!lexer->has_peeked) {
		lexer->peeked = _token_next(lexer);
		lexer->has_peeked = true;
	}
	
	return lexer->peeked;
}

void lexer_dump(lexer_t *lexer, FILE *stream) {
	if (!lexer) {
		die("lexer_dump(): null lexer");
	}

	if (!stream) {
		die("lexer_dump(): null stream");
	}
	
	token_t token = lexer_next(lexer);
	
	fprintf(stream, "[ ");
	
	while (token.type != TOK_EOF) {
		switch (token.type) {
			case TOK_EOF: {
				die("lexer_dump(): unexpectoed EOF");
			}
			
			case TOK_UNKNOWN: {
				die("lexer_dump(): unknown lexeme");
			}
			
			case TOK_NUM: {
				fprintf(stream, "TOK_NUM (%g)", token.value.num);
				break;
			}
			
			case TOK_ADD: {
				fprintf(stream, "TOK_ADD (+)");
				break;
			}
			
			case TOK_SUB: {
				fprintf(stream, "TOK_SUB (-)");
				break;
			}
			
			case TOK_MUL: {
				fprintf(stream, "TOK_MUL (*)");
				break;
			}
			
			case TOK_DIV: {
				fprintf(stream, "TOK_DIV (/)");
				break;
			}
			
			case TOK_POW: {
				fprintf(stream, "TOK_POW (^)");
				break;
			}
			
			case TOK_LPAREN: {
				fprintf(stream, "TOK_LPAREN (()");
				break;
			}
			
			case TOK_RPAREN: {
				fprintf(stream, "TOK_RPAREN ())");
				break;
			}
			
			case TOK_PRINT: {
				fprintf(stream, "TOK_PRINT (print)");
				break;
			}

			case TOK_LET: {
				fprintf(stream, "TOK_LET (let)");
				break;
			}

			case TOK_COLON: {
				fprintf(stream, "TOK_COLON (:)");
				break;
			}

			case TOK_SCOLON: {
				fprintf(stream, "TOK_SCOLON (;)");
				break;
			}


			case TOK_IDENT: {
				fprintf(stream, "TOK_IDENT (%s)", token.value.str);
				break;
			}

			default: {
				die("lexer_dump(): unknown token");
			}
		}
		
		token = lexer_next(lexer);
		fprintf(stream, ",\n");
	}
	
	fprintf(stream, "TOK_EOF ]\n");
}

