#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

/* Returns 0 on success, non-zero if semantic errors found */
int semantic_check(ASTNode *root);

#endif
