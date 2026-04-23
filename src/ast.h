#ifndef AST_H
#define AST_H

/* Node types for the AST */
typedef enum {
    NODE_STMTLIST,
    NODE_DECL,
    NODE_ASSIGN,
    NODE_IF,
    NODE_IFELSE,
    NODE_WHILE,
    NODE_FOR,
    NODE_PRINT,
    NODE_RETURN,
    /* Arithmetic */
    NODE_ADD, NODE_SUB, NODE_MUL, NODE_DIV, NODE_NEG,
    /* Comparison */
    NODE_EQ, NODE_NEQ, NODE_LT, NODE_GT, NODE_LE, NODE_GE,
    /* Logical */
    NODE_AND, NODE_OR, NODE_NOT,
    /* Literals & Identifiers */
    NODE_INT, NODE_FLOAT, NODE_ID
} NodeType;

typedef enum { TYPE_INT, TYPE_FLOAT, TYPE_UNKNOWN } DataType;

typedef struct ASTNode {
    NodeType type;
    DataType dtype;

    /* For literals */
    int   ival;
    float fval;

    /* For identifiers / declarations */
    char *name;

    /* Children */
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *extra;   /* used by for-loop (init/incr) and if-else */
} ASTNode;

/* Constructors */
ASTNode *make_node(NodeType type, ASTNode *left, ASTNode *right);
ASTNode *make_int_node(int val);
ASTNode *make_float_node(float val);
ASTNode *make_id_node(char *name);
ASTNode *make_decl_node(char *name, DataType dtype, ASTNode *init);
ASTNode *make_assign_node(char *name, ASTNode *expr);
ASTNode *make_ifelse_node(ASTNode *cond, ASTNode *then_b, ASTNode *else_b);
ASTNode *make_for_node(ASTNode *init, ASTNode *cond, ASTNode *incr, ASTNode *body);

/* Print AST */
void print_ast(ASTNode *node, int depth);
void free_ast(ASTNode *node);

#endif
