# Mini Compiler for DSL (Basic Programming Language)
### Compiler Design Assignment — CSE Department

---

## 📌 Project Description

This project implements a **mini compiler** for a custom Domain Specific Language (DSL) that supports basic programming constructs: variable declarations, arithmetic expressions, if/else conditionals, while loops, and for loops.

The compiler is built in **C** using **Flex** (lexical analysis) and **Bison** (parsing), and covers all five required compiler stages:

| Stage | Tool/Module |
|-------|------------|
| Lexical Analysis | Flex (`lexer.l`) |
| Parsing | Bison (`parser.y`) |
| AST Construction | C (`ast.c / ast.h`) |
| Semantic Analysis | C (`semantic.c`) |
| Intermediate Code Generation | C (`codegen.c`) — Three-Address Code (TAC) |

---

## 🛠️ Tools Used

- **Flex** — Lexical analyzer generator
- **Bison** — LALR parser generator
- **GCC** — C compiler
- **Make** — Build system
- **Linux/Ubuntu** — Development environment

---

## 📁 Folder Structure

```
Compiler-Design-DSL/
│── src/
│   ├── lexer.l          # Flex lexer (tokenizer)
│   ├── parser.y         # Bison grammar + AST construction
│   ├── ast.h / ast.c    # AST node definitions and builders
│   ├── symtable.h / symtable.c  # Symbol table for semantic analysis
│   ├── semantic.h / semantic.c  # Semantic analysis (type checking)
│   ├── codegen.h / codegen.c    # Three-Address Code generation
│   └── main.c           # Compiler driver
│── docs/
│   └── report.pdf       # Architecture & module explanation
│── test/
│   ├── test1.dsl        # Variables & arithmetic
│   ├── test2.dsl        # If-else conditions
│   └── test3.dsl        # While and for loops
│── output/
│   ├── test1.tac        # TAC output for test1
│   ├── test2.tac        # TAC output for test2
│   └── test3.tac        # TAC output for test3
│── Makefile
└── README.md
```

---

## ▶️ Steps to Run

### Prerequisites
```bash
sudo apt-get install flex bison gcc make
```

### Build
```bash
# Step 1: Generate lexer and parser source files
bison -d -o src/parser.tab.c src/parser.y
flex  -o src/lex.yy.c src/lexer.l

# Step 2: Compile everything
make

# OR manually:
gcc -Wall -g -Isrc -c src/ast.c       -o obj/ast.o
gcc -Wall -g -Isrc -c src/symtable.c  -o obj/symtable.o
gcc -Wall -g -Isrc -c src/semantic.c  -o obj/semantic.o
gcc -Wall -g -Isrc -c src/codegen.c   -o obj/codegen.o
gcc -Wall -g -Isrc -c src/parser.tab.c -o obj/parser.tab.o
gcc -Wall -g -Isrc -c src/lex.yy.c    -o obj/lex.yy.o
gcc -Wall -g -Isrc -c src/main.c      -o obj/main.o
gcc -Wall -g -Isrc -o compiler obj/*.o -lfl
```

### Run
```bash
./compiler test/test1.dsl output/test1.tac
./compiler test/test2.dsl output/test2.tac
./compiler test/test3.dsl output/test3.tac
```

---

## 📝 DSL Language Syntax

### Variable Declaration
```
int x = 10;
float pi = 3.14;
```

### Arithmetic
```
x = x + y * 2;
```

### If / Else
```
if (x > y) {
    max = x;
} else {
    max = y;
}
```

### While Loop
```
while (i <= 10) {
    sum = sum + i;
    i = i + 1;
}
```

### For Loop
```
for (i = 0; i < 5; i = i + 1) {
    print(i);
}
```

### Print
```
print(x);
```

---

## 🧪 Sample Input / Output

### Input (`test2.dsl`):
```
int x = 50;
int y = 30;
int max = 0;

if (x > y) {
    max = x;
} else {
    max = y;
}
print(max);
```

### Output (`test2.tac`):
```
// === Three-Address Intermediate Code ===
BEGIN:
    t0 = 50
    x = t0
    t1 = 30
    y = t1
    t2 = 0
    max = t2
    t3 = x > y
    ifFalse t3 goto L0
    max = x
    goto L1
L0:
    max = y
L1:
    print max
END:
```

---

## 👥 Team Details

| Field | Details |
|-------|---------|
| Subject | Compiler Design |
| Department | CSE |
| Faculty | Dr. Gunasundari C, Asst. Prof |
| Assignment | Mini Compiler Project |

---

## 📊 Marks Breakdown

| Criteria | Marks |
|----------|-------|
| Functionality | 2 |
| Implementation | 1 |
| Code Quality | 1 |
| Documentation | 1 |
| **Total** | **5** |
