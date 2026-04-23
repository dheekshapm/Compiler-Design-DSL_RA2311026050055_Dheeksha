#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static ASTNode *alloc_node(NodeType type) {
    ASTNode *n = calloc(1, sizeof(ASTNode));
    if (!n) { fprintf(stderr, "Out of memory\n"); exit(1); }
    n->type  = type;
    n->dtype = TYPE_UNKNOWN;
    return n;
}

ASTNode *make_node(NodeType type, ASTNode *left, ASTNode *right) {
    ASTNode *n = alloc_node(type);
    n->left  = left;
    n->right = right;
    return n;
}

ASTNode *make_int_node(int val) {
    ASTNode *n = alloc_node(NODE_INT);
    n->ival  = val;
    n->dtype = TYPE_INT;
    return n;
}

ASTNode *make_float_node(float val) {
    ASTNode *n = alloc_node(NODE_FLOAT);
    n->fval  = val;
    n->dtype = TYPE_FLOAT;
    return n;
}

ASTNode *make_id_node(char *name) {
    ASTNode *n = alloc_node(NODE_ID);
    n->name = strdup(name);
    return n;
}

ASTNode *make_decl_node(char *name, DataType dtype, ASTNode *init) {
    ASTNode *n = alloc_node(NODE_DECL);
    n->name  = strdup(name);
    n->dtype = dtype;
    n->left  = init;
    return n;
}

ASTNode *make_assign_node(char *name, ASTNode *expr) {
    ASTNode *n = alloc_node(NODE_ASSIGN);
    n->name  = strdup(name);
    n->left  = expr;
    return n;
}

ASTNode *make_ifelse_node(ASTNode *cond, ASTNode *then_b, ASTNode *else_b) {
    ASTNode *n = alloc_node(NODE_IFELSE);
    n->left  = cond;
    n->right = then_b;
    n->extra = else_b;
    return n;
}

ASTNode *make_for_node(ASTNode *init, ASTNode *cond, ASTNode *incr, ASTNode *body) {
    ASTNode *n = alloc_node(NODE_FOR);
    n->left  = init;
    n->right = cond;
    n->extra = make_node(NODE_STMTLIST, incr, body);
    return n;
}

static const char *node_name(NodeType t) {
    switch(t) {
        case NODE_STMTLIST: return "StmtList";
        case NODE_DECL:     return "Decl";
        case NODE_ASSIGN:   return "Assign";
        case NODE_IF:       return "If";
        case NODE_IFELSE:   return "IfElse";
        case NODE_WHILE:    return "While";
        case NODE_FOR:      return "For";
        case NODE_PRINT:    return "Print";
        case NODE_RETURN:   return "Return";
        case NODE_ADD:      return "Add";
        case NODE_SUB:      return "Sub";
        case NODE_MUL:      return "Mul";
        case NODE_DIV:      return "Div";
        case NODE_NEG:      return "Neg";
        case NODE_EQ:       return "Eq";
        case NODE_NEQ:      return "Neq";
        case NODE_LT:       return "Lt";
        case NODE_GT:       return "Gt";
        case NODE_LE:       return "Le";
        case NODE_GE:       return "Ge";
        case NODE_AND:      return "And";
        case NODE_OR:       return "Or";
        case NODE_NOT:      return "Not";
        case NODE_INT:      return "IntLit";
        case NODE_FLOAT:    return "FloatLit";
        case NODE_ID:       return "Id";
        default:            return "Unknown";
    }
}

void print_ast(ASTNode *node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("[%s]", node_name(node->type));
    if (node->type == NODE_INT)   printf(" %d", node->ival);
    if (node->type == NODE_FLOAT) printf(" %.2f", node->fval);
    if (node->name)               printf(" '%s'", node->name);
    printf("\n");
    print_ast(node->left,  depth + 1);
    print_ast(node->right, depth + 1);
    print_ast(node->extra, depth + 1);
}

void free_ast(ASTNode *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->extra);
    if (node->name) free(node->name);
    free(node);
}
