#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <stdio.h>

/* Generate Three-Address Code (TAC) intermediate representation */
void codegen(ASTNode *root, FILE *out);

#endif
