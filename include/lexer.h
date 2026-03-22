#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#define MAX_IDENT_LEN 32

typedef enum {
	TOK_NUM,    
	TOK_ADD,    
	TOK_SUB,    
	TOK_MUL,    
	TOK_DIV,    
	TOK_POW,
	TOK_LPAREN,    
	TOK_RPAREN,    
	TOK_PRINT,
	TOK_COLON,
	TOK_SCOLON,
	TOK_LET,
	TOK_IDENT,
	TOK_EOF,    
	TOK_UNKNOWN
} token_type_t; 

typedef struct {
	token_type_t type;
	
	union {
		double num;
		char str[MAX_IDENT_LEN + 1];
	} value;
} token_t;

typedef		struct _lexer lexer_t;

/*
- BRIEF:	create a new lexer structure
- PARAM:	source program
- RET:		new lexer
*/
lexer_t		*lexer_new(const char *src);

/*
- BRIEF:	free lexer
- PARAM:	address of lexer
- RET:		void
- NOTE:		sets the lexer to NULL
*/
void		lexer_free(lexer_t **lexer_addr);

/*
- BRIEF:	consume next token
- PARAM:	lexer
- RET:		next token
*/
token_t		lexer_next(lexer_t *lexer);

/*
- BRIEF:	peek next token (without consuming)
- PARAM:	lexer
- RET:		next token
*/
token_t		lexer_peek(lexer_t *lexer);

/*
- BRIEF:	dump all tokens to a stream
- PARAM:	lexer, stream
- RET:		void
*/
void		lexer_dump(lexer_t *lexer, FILE *stream);

#endif

