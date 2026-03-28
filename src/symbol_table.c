#include "../include/common.h"
#include "../include/symbol_table.h"

#include <string.h>

struct _symbol {
	char name[MAX_IDENT_LEN + 1];
	double value;
};

struct _symbol_table_t {
	symbol_t *symbols;
	size_t count;
	size_t capacity;
};

symbol_table_t *symbol_table_new(size_t capacity) {
        if (capacity == 0) {
                die("symbol_table_new(): 0 symbol capacity");
        }

        symbol_table_t *table = mem_alloc(sizeof(symbol_table_t));

        table->symbols = mem_alloc(sizeof(symbol_t) * capacity);
        table->count = 0;
        table->capacity = capacity;

        return table;
}

symbol_t *symbol_lookup(symbol_table_t *table, const char *name) {
        if (!table) {
                die("symbol_lookup(): null table");
        }

        if (!name) {
                die("symbol_lookup(): null name");
        }

        for (size_t idx = 0; idx < table->count; idx++) {
                if (strcmp(table->symbols[idx].name, name) == 0) {
                        return &table->symbols[idx];
                }
        }

        return NULL;
}

void symbol_insert(symbol_table_t *table, const char *name, double value) {
        if (!table) {
                die("symbol_insert(): null table");
        }

        if (!name) {
                die("symbol_insert(): null name");
        }

        symbol_t *symbol = symbol_lookup(table, name);

        if (symbol) {
                symbol->value = value;
                return;
        }

        if (table->capacity <= table->count) {
                die("symbol_insert(): symbol table full");
        }

        strcpy(table->symbols[table->count].name, name);
        table->symbols[table->count].value = value;
        table->count++;
}

