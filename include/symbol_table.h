#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../include/lexer.h"

typedef struct _symbol {
	char name[MAX_IDENT_LEN + 1];
	double value;
} symbol_t;

typedef struct _symbol_table_t symbol_table_t; 

symbol_table_t *symbol_table_new(size_t capacity);
symbol_t *symbol_lookup(symbol_table_t *table, const char *name);
void symbol_insert(symbol_table_t *table, const char *name, double value);

#endif

