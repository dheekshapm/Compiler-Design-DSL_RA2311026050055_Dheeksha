#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

static Symbol table[MAX_SYMBOLS];
static int    sym_count = 0;

void sym_init() {
    sym_count = 0;
}

int sym_insert(const char *name, DataType type) {
    if (sym_lookup(name)) {
        fprintf(stderr, "Semantic Error: Variable '%s' already declared.\n", name);
        return -1;
    }
    if (sym_count >= MAX_SYMBOLS) {
        fprintf(stderr, "Symbol table full.\n");
        return -1;
    }
    table[sym_count].name    = strdup(name);
    table[sym_count].type    = type;
    table[sym_count].is_init = 0;
    table[sym_count].value   = 0;
    return sym_count++;
}

Symbol *sym_lookup(const char *name) {
    for (int i = 0; i < sym_count; i++)
        if (strcmp(table[i].name, name) == 0)
            return &table[i];
    return NULL;
}

void sym_set_value(const char *name, float val) {
    Symbol *s = sym_lookup(name);
    if (s) { s->value = val; s->is_init = 1; }
}

void sym_print_table() {
    printf("\n===== Symbol Table =====\n");
    printf("%-15s %-10s %-10s %-10s\n", "Name", "Type", "Init", "Value");
    printf("----------------------------------------\n");
    for (int i = 0; i < sym_count; i++) {
        printf("%-15s %-10s %-10s ",
            table[i].name,
            table[i].type == TYPE_INT ? "int" : "float",
            table[i].is_init ? "yes" : "no");
        if (table[i].type == TYPE_INT)
            printf("%-10d\n", (int)table[i].value);
        else
            printf("%-10.2f\n", table[i].value);
    }
    printf("========================\n\n");
}
