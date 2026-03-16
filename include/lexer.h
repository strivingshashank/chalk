#ifndef LEXER_H
#define LEXER_H

typedef enum {
	TOK_NUM,    
	TOK_ADD,    
	TOK_SUB,    
	TOK_MUL,    
	TOK_DIV,    
	TOK_POW,
	TOK_LPAREN,    
	TOK_RPAREN,    
	TOK_EOF,    
	TOK_UNKNOWN
} token_type_t; 

typedef struct {
	token_type_t type;
	double       value;
} token_t;

typedef struct lexer lexer_t;

/*
- BRIEF:  create a new lexer structure
- PARAM:  expression string
- RET:    new lexer
*/
lexer_t *lexer_new(const char *src);

/*
- BRIEF:  free lexer
- PARAM:  address of lexer
- RET:    void
- NOTE:   sets the lexer to NULL
*/
void     lexer_free(lexer_t **lexer_addr);

/*
- BRIEF:  consume next token
- PARAM:  lexer
- RET:    next token
*/
token_t  lexer_next(lexer_t *lexer);

/*
- BRIEF:  peek next token (without consuming)
- PARAM:  lexer
- RET:    next token
*/
token_t  lexer_peek(lexer_t *lexer);

void lexer_dump(lexer_t *lexer);

#endif

