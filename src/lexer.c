#include "../include/lexer.h"
#include "../include/common.h"

#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>

struct lexer {
	const char *src;
	size_t      pos;
	token_t     peeked;
	bool        has_peeked;
};

static token_t token_next(lexer_t *lexer) {
	if (!lexer) {
		die("token_next(): null lexer");
	}
	
	/* skip whitespace */
	while (isspace(lexer->src[lexer->pos])) {
		lexer->pos++;
	}
	
	/* handle NULL */
	if (lexer->src[lexer->pos] == '\0') {
		return (token_t){ .type = TOK_EOF, .value = 0 };
	}
	
	/* form number */
	if (isdigit(lexer->src[lexer->pos])) {
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
		
		return (token_t){ .type = TOK_NUM, .value = value };
	}
	
	lexer->pos++;
	
	switch (lexer->src[lexer->pos - 1]) {
		case '+': return (token_t){ .type = TOK_ADD, .value = 0 };
		case '-': return (token_t){ .type = TOK_SUB, .value = 0 };
		case '*': return (token_t){ .type = TOK_MUL, .value = 0 };
		case '/': return (token_t){ .type = TOK_DIV, .value = 0 };
		case '^': return (token_t){ .type = TOK_POW, .value = 0 };
		case '(': return (token_t){ .type = TOK_LPAREN, .value = 0 };
		case ')': return (token_t){ .type = TOK_RPAREN, .value = 0 };
		default:  return (token_t){ .type = TOK_UNKNOWN, .value = 0 };
	}
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
	
	return token_next(lexer);
}

token_t lexer_peek(lexer_t *lexer) {
	if (!lexer) {
		die("lexer_peek(): null lexer");
	}
	
	if (!lexer->has_peeked) {
		lexer->peeked = token_next(lexer);
		lexer->has_peeked = true;
	}
	
	return lexer->peeked;
}

void lexer_dump(lexer_t *lexer) {
	if (!lexer) {
		die("lexer_dump(): null lexer");
	}
	
	token_t token = lexer_next(lexer);
	
	printf("[ ");
	
	while (token.type != TOK_EOF) {
		switch (token.type) {
			case TOK_EOF: {
				die("lexer_dump(): unexpectoed EOF");
			}
			
			case TOK_UNKNOWN: {
				die("lexer_dump(): unknown lexeme");
			}
			
			case TOK_NUM: {
				printf("TOK_NUM (%g)", token.value);
				break;
			}
			
			case TOK_ADD: {
				printf("TOK_ADD (+)");
				break;
			}
			
			case TOK_SUB: {
				printf("TOK_SUB (-)");
				break;
			}
			
			case TOK_MUL: {
				printf("TOK_MUL (*)");
				break;
			}
			
			case TOK_DIV: {
				printf("TOK_DIV (/)");
				break;
			}
			
			case TOK_POW: {
				printf("TOK_POW (^)");
				break;
			}
			
			case TOK_LPAREN: {
				printf("TOK_LPAREN (()");
				break;
			}
			
			case TOK_RPAREN: {
				printf("TOK_RPAREN ())");
				break;
			}
			
		}
		
		token = lexer_next(lexer);
		printf(", ");
	}
	
	printf("TOK_EOF ]\n");
}

