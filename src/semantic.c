#include <stdio.h>
#include <stdlib.h>
#include "semantic.h"
#include "symtable.h"

static int errors = 0;

static DataType check_expr(ASTNode *node);

static DataType check_expr(ASTNode *node) {
    if (!node) return TYPE_UNKNOWN;

    switch (node->type) {
        case NODE_INT:   return TYPE_INT;
        case NODE_FLOAT: return TYPE_FLOAT;

        case NODE_ID: {
            Symbol *s = sym_lookup(node->name);
            if (!s) {
                fprintf(stderr, "Semantic Error: Undeclared variable '%s'\n", node->name);
                errors++;
                return TYPE_UNKNOWN;
            }
            if (!s->is_init) {
                fprintf(stderr, "Semantic Warning: Variable '%s' used before initialization\n", node->name);
            }
            return s->type;
        }

        case NODE_ADD: case NODE_SUB:
        case NODE_MUL: case NODE_DIV: {
            DataType lt = check_expr(node->left);
            DataType rt = check_expr(node->right);
            if (lt == TYPE_FLOAT || rt == TYPE_FLOAT) return TYPE_FLOAT;
            return TYPE_INT;
        }

        case NODE_NEG:
            return check_expr(node->left);

        case NODE_EQ: case NODE_NEQ: case NODE_LT:
        case NODE_GT: case NODE_LE:  case NODE_GE:
        case NODE_AND: case NODE_OR: case NODE_NOT:
            check_expr(node->left);
            check_expr(node->right);
            return TYPE_INT;  /* bool treated as int */

        default:
            return TYPE_UNKNOWN;
    }
}

static void check_node(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_STMTLIST:
            check_node(node->left);
            check_node(node->right);
            break;

        case NODE_DECL: {
            sym_insert(node->name, node->dtype);
            if (node->left) {
                DataType et = check_expr(node->left);
                if (et != TYPE_UNKNOWN && et != node->dtype) {
                    fprintf(stderr,
                        "Semantic Warning: Type mismatch in declaration of '%s'\n",
                        node->name);
                }
                sym_set_value(node->name, 0); /* mark initialized */
            }
            break;
        }

        case NODE_ASSIGN: {
            Symbol *s = sym_lookup(node->name);
            if (!s) {
                fprintf(stderr, "Semantic Error: Assigning to undeclared variable '%s'\n", node->name);
                errors++;
            } else {
                DataType et = check_expr(node->left);
                if (et != TYPE_UNKNOWN && s->type != et) {
                    fprintf(stderr,
                        "Semantic Warning: Possible type mismatch in assignment to '%s'\n",
                        node->name);
                }
                s->is_init = 1;
            }
            break;
        }

        case NODE_IF:
        case NODE_WHILE:
            check_expr(node->left);
            check_node(node->right);
            break;

        case NODE_IFELSE:
            check_expr(node->left);
            check_node(node->right);
            check_node(node->extra);
            break;

        case NODE_FOR:
            check_node(node->left);
            check_expr(node->right);
            if (node->extra) {
                check_node(node->extra->left);
                check_node(node->extra->right);
            }
            break;

        case NODE_PRINT:
            check_expr(node->left);
            break;

        case NODE_RETURN:
            check_expr(node->left);
            break;

        default:
            check_expr(node);
            break;
    }
}

int semantic_check(ASTNode *root) {
    errors = 0;
    sym_init();
    check_node(root);
    if (errors == 0)
        printf("Semantic Analysis: OK (no errors)\n");
    else
        fprintf(stderr, "Semantic Analysis: %d error(s) found\n", errors);
    return errors;
}
