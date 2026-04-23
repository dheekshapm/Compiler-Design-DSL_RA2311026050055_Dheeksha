#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "ast.h"

#define MAX_SYMBOLS 256

typedef struct {
    char    *name;
    DataType type;
    int      is_init;
    float    value;   /* stores int or float */
} Symbol;

void   sym_init();
int    sym_insert(const char *name, DataType type);
Symbol *sym_lookup(const char *name);
void   sym_set_value(const char *name, float val);
void   sym_print_table();

#endif
