CC     = gcc
CFLAGS = -Wall -g -Isrc
LEX    = flex
YACC   = bison

SRC = src
OBJ = obj

all: compiler

compiler: $(OBJ)/parser.tab.o $(OBJ)/lex.yy.o $(OBJ)/ast.o \
          $(OBJ)/symtable.o $(OBJ)/semantic.o $(OBJ)/codegen.o $(OBJ)/main.o
	$(CC) $(CFLAGS) -o compiler $^ -lfl

$(OBJ)/parser.tab.o: $(SRC)/parser.tab.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/lex.yy.o: $(SRC)/lex.yy.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/ast.o: $(SRC)/ast.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/symtable.o: $(SRC)/symtable.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/semantic.o: $(SRC)/semantic.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/codegen.o: $(SRC)/codegen.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/main.o: $(SRC)/main.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

# Generate parser and lexer
generate:
	$(YACC) -d -o $(SRC)/parser.tab.c $(SRC)/parser.y
	$(LEX) -o $(SRC)/lex.yy.c $(SRC)/lexer.l

clean:
	rm -rf $(OBJ) compiler $(SRC)/parser.tab.c $(SRC)/parser.tab.h $(SRC)/lex.yy.c

run: compiler
	mkdir -p output
	./compiler test/test1.dsl output/output.tac

.PHONY: all clean generate run
