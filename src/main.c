#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symtable.h"
#include "semantic.h"
#include "codegen.h"

extern int  yyparse();
extern FILE *yyin;
extern ASTNode *root;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file> [output_file]\n", argv[0]);
        return 1;
    }

    /* Open source file */
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        fprintf(stderr, "Error: Cannot open '%s'\n", argv[1]);
        return 1;
    }

    printf("========================================\n");
    printf("   Mini Compiler - DSL for Basic Lang   \n");
    printf("========================================\n\n");

    /* --- Stage 1: Lexical + Syntax Analysis (via yyparse) --- */
    printf("[Stage 1] Lexical & Syntax Analysis...\n");
    int parse_result = yyparse();
    fclose(yyin);

    if (parse_result != 0) {
        fprintf(stderr, "Compilation failed at parsing stage.\n");
        return 1;
    }
    printf("           Parsing: OK\n\n");

    /* --- Stage 2: AST --- */
    printf("[Stage 2] Abstract Syntax Tree:\n");
    print_ast(root, 1);
    printf("\n");

    /* --- Stage 3: Semantic Analysis --- */
    printf("[Stage 3] Semantic Analysis...\n");
    int sem_errors = semantic_check(root);
    sym_print_table();
    if (sem_errors > 0) {
        fprintf(stderr, "Compilation failed at semantic stage.\n");
        free_ast(root);
        return 1;
    }

    /* --- Stage 4: Intermediate Code Generation --- */
    printf("[Stage 4] Intermediate Code Generation (Three-Address Code)...\n");
    const char *outfile = (argc >= 3) ? argv[2] : "output/output.tac";
    FILE *out = fopen(outfile, "w");
    if (!out) {
        /* try current dir */
        out = fopen("output.tac", "w");
        outfile = "output.tac";
    }
    if (!out) {
        fprintf(stderr, "Error: Cannot open output file\n");
        free_ast(root);
        return 1;
    }

    codegen(root, out);
    fclose(out);

    /* Also print to stdout */
    FILE *f = fopen(outfile, "r");
    if (f) {
        char line[256];
        while (fgets(line, sizeof(line), f)) printf("%s", line);
        fclose(f);
    }

    printf("\nCompilation successful! TAC written to: %s\n", outfile);
    printf("========================================\n");

    free_ast(root);
    return 0;
}
