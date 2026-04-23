#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

static int temp_count  = 0;
static int label_count = 0;

static char *new_temp() {
    char *t = malloc(16);
    sprintf(t, "t%d", temp_count++);
    return t;
}

static char *new_label() {
    char *l = malloc(16);
    sprintf(l, "L%d", label_count++);
    return l;
}

/* Returns the temp/variable holding the result of expr */
static char *gen_expr(ASTNode *node, FILE *out) {
    if (!node) return NULL;

    switch (node->type) {
        case NODE_INT: {
            char *t = new_temp();
            fprintf(out, "\t%s = %d\n", t, node->ival);
            return t;
        }
        case NODE_FLOAT: {
            char *t = new_temp();
            fprintf(out, "\t%s = %.2f\n", t, node->fval);
            return t;
        }
        case NODE_ID:
            return strdup(node->name);

        case NODE_NEG: {
            char *v = gen_expr(node->left, out);
            char *t = new_temp();
            fprintf(out, "\t%s = -%s\n", t, v);
            free(v);
            return t;
        }

        case NODE_ADD: case NODE_SUB:
        case NODE_MUL: case NODE_DIV: {
            char *l = gen_expr(node->left,  out);
            char *r = gen_expr(node->right, out);
            char *t = new_temp();
            char op = node->type == NODE_ADD ? '+' :
                      node->type == NODE_SUB ? '-' :
                      node->type == NODE_MUL ? '*' : '/';
            fprintf(out, "\t%s = %s %c %s\n", t, l, op, r);
            free(l); free(r);
            return t;
        }

        case NODE_EQ:  case NODE_NEQ: case NODE_LT:
        case NODE_GT:  case NODE_LE:  case NODE_GE: {
            char *l = gen_expr(node->left,  out);
            char *r = gen_expr(node->right, out);
            char *t = new_temp();
            const char *op =
                node->type == NODE_EQ  ? "==" :
                node->type == NODE_NEQ ? "!=" :
                node->type == NODE_LT  ? "<"  :
                node->type == NODE_GT  ? ">"  :
                node->type == NODE_LE  ? "<=" : ">=";
            fprintf(out, "\t%s = %s %s %s\n", t, l, op, r);
            free(l); free(r);
            return t;
        }

        case NODE_AND: {
            char *l = gen_expr(node->left,  out);
            char *r = gen_expr(node->right, out);
            char *t = new_temp();
            fprintf(out, "\t%s = %s && %s\n", t, l, r);
            free(l); free(r);
            return t;
        }
        case NODE_OR: {
            char *l = gen_expr(node->left,  out);
            char *r = gen_expr(node->right, out);
            char *t = new_temp();
            fprintf(out, "\t%s = %s || %s\n", t, l, r);
            free(l); free(r);
            return t;
        }
        case NODE_NOT: {
            char *v = gen_expr(node->left, out);
            char *t = new_temp();
            fprintf(out, "\t%s = !%s\n", t, v);
            free(v);
            return t;
        }

        default:
            return strdup("?");
    }
}

static void gen_stmt(ASTNode *node, FILE *out);

static void gen_stmt(ASTNode *node, FILE *out) {
    if (!node) return;

    switch (node->type) {
        case NODE_STMTLIST:
            gen_stmt(node->left,  out);
            gen_stmt(node->right, out);
            break;

        case NODE_DECL:
            if (node->left) {
                char *v = gen_expr(node->left, out);
                fprintf(out, "\t%s = %s\n", node->name, v);
                free(v);
            } else {
                fprintf(out, "\t%s = 0  // declared\n", node->name);
            }
            break;

        case NODE_ASSIGN: {
            char *v = gen_expr(node->left, out);
            fprintf(out, "\t%s = %s\n", node->name, v);
            free(v);
            break;
        }

        case NODE_IF: {
            char *Lfalse = new_label();
            char *Lend   = new_label();
            char *cond   = gen_expr(node->left, out);
            fprintf(out, "\tifFalse %s goto %s\n", cond, Lfalse);
            free(cond);
            gen_stmt(node->right, out);
            fprintf(out, "\tgoto %s\n", Lend);
            fprintf(out, "%s:\n", Lfalse);
            fprintf(out, "%s:\n", Lend);
            free(Lfalse); free(Lend);
            break;
        }

        case NODE_IFELSE: {
            char *Lelse = new_label();
            char *Lend  = new_label();
            char *cond  = gen_expr(node->left, out);
            fprintf(out, "\tifFalse %s goto %s\n", cond, Lelse);
            free(cond);
            gen_stmt(node->right, out);       /* then branch */
            fprintf(out, "\tgoto %s\n", Lend);
            fprintf(out, "%s:\n", Lelse);
            gen_stmt(node->extra, out);       /* else branch */
            fprintf(out, "%s:\n", Lend);
            free(Lelse); free(Lend);
            break;
        }

        case NODE_WHILE: {
            char *Lstart = new_label();
            char *Lend   = new_label();
            fprintf(out, "%s:\n", Lstart);
            char *cond = gen_expr(node->left, out);
            fprintf(out, "\tifFalse %s goto %s\n", cond, Lend);
            free(cond);
            gen_stmt(node->right, out);
            fprintf(out, "\tgoto %s\n", Lstart);
            fprintf(out, "%s:\n", Lend);
            free(Lstart); free(Lend);
            break;
        }

        case NODE_FOR: {
            char *Lstart = new_label();
            char *Lend   = new_label();
            gen_stmt(node->left, out);           /* init */
            fprintf(out, "%s:\n", Lstart);
            char *cond = gen_expr(node->right, out); /* condition */
            fprintf(out, "\tifFalse %s goto %s\n", cond, Lend);
            free(cond);
            if (node->extra) {
                gen_stmt(node->extra->right, out);   /* body */
                gen_stmt(node->extra->left,  out);   /* increment */
            }
            fprintf(out, "\tgoto %s\n", Lstart);
            fprintf(out, "%s:\n", Lend);
            free(Lstart); free(Lend);
            break;
        }

        case NODE_PRINT: {
            char *v = gen_expr(node->left, out);
            fprintf(out, "\tprint %s\n", v);
            free(v);
            break;
        }

        case NODE_RETURN: {
            char *v = gen_expr(node->left, out);
            fprintf(out, "\treturn %s\n", v);
            free(v);
            break;
        }

        default:
            break;
    }
}

void codegen(ASTNode *root, FILE *out) {
    temp_count  = 0;
    label_count = 0;
    fprintf(out, "// === Three-Address Intermediate Code ===\n");
    fprintf(out, "BEGIN:\n");
    gen_stmt(root, out);
    fprintf(out, "END:\n");
}
