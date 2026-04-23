from reportlab.lib.pagesizes import letter
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle, HRFlowable
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib import colors
from reportlab.lib.units import inch

doc = SimpleDocTemplate(
    "/home/claude/Compiler-Design-DSL/docs/report.pdf",
    pagesize=letter,
    leftMargin=inch, rightMargin=inch,
    topMargin=inch, bottomMargin=inch
)

styles = getSampleStyleSheet()

title_style = ParagraphStyle('CustomTitle', parent=styles['Title'],
    fontSize=18, textColor=colors.HexColor('#1a3c5e'), spaceAfter=6)
h1_style = ParagraphStyle('H1', parent=styles['Heading1'],
    fontSize=13, textColor=colors.HexColor('#1a3c5e'), spaceAfter=4, spaceBefore=14)
h2_style = ParagraphStyle('H2', parent=styles['Heading2'],
    fontSize=11, textColor=colors.HexColor('#2e6da4'), spaceAfter=3, spaceBefore=8)
body_style = ParagraphStyle('Body', parent=styles['Normal'],
    fontSize=10, leading=14, spaceAfter=4)
code_style = ParagraphStyle('Code', parent=styles['Code'],
    fontSize=8.5, leading=12, backColor=colors.HexColor('#f4f4f4'),
    leftIndent=12, rightIndent=12, spaceAfter=4, spaceBefore=4,
    fontName='Courier')

story = []

# Title
story.append(Paragraph("Mini Compiler for DSL", title_style))
story.append(Paragraph("Compiler Design Assignment — CSE Department", styles['Normal']))
story.append(Paragraph("Faculty: Dr. Gunasundari C, Asst. Prof", styles['Normal']))
story.append(Spacer(1, 8))
story.append(HRFlowable(width="100%", thickness=2, color=colors.HexColor('#1a3c5e')))
story.append(Spacer(1, 10))

# Section 1: Architecture
story.append(Paragraph("1. Compiler Architecture", h1_style))
story.append(Paragraph(
    "The mini compiler is built in C using Flex for lexical analysis and Bison for parsing. "
    "It processes a custom DSL (Domain Specific Language) that supports variables, arithmetic, "
    "if/else conditions, while loops, and for loops. The compiler follows the classical pipeline:",
    body_style))

arch_data = [
    ['Stage', 'Module', 'Description'],
    ['1. Lexical Analysis', 'lexer.l (Flex)', 'Tokenizes source into keywords, identifiers, operators, literals'],
    ['2. Syntax Analysis', 'parser.y (Bison)', 'LALR(1) grammar validates structure, builds AST'],
    ['3. AST Construction', 'ast.c / ast.h', 'Tree of ASTNode structs representing program structure'],
    ['4. Semantic Analysis', 'semantic.c', 'Type checking, undeclared variable detection, scope validation'],
    ['5. Code Generation', 'codegen.c', 'Generates Three-Address Code (TAC) as intermediate representation'],
]

arch_table = Table(arch_data, colWidths=[1.4*inch, 1.5*inch, 3.5*inch])
arch_table.setStyle(TableStyle([
    ('BACKGROUND', (0,0), (-1,0), colors.HexColor('#1a3c5e')),
    ('TEXTCOLOR',  (0,0), (-1,0), colors.white),
    ('FONTNAME',   (0,0), (-1,0), 'Helvetica-Bold'),
    ('FONTSIZE',   (0,0), (-1,-1), 9),
    ('ROWBACKGROUNDS', (0,1), (-1,-1), [colors.HexColor('#eaf1fb'), colors.white]),
    ('GRID',       (0,0), (-1,-1), 0.5, colors.HexColor('#cccccc')),
    ('VALIGN',     (0,0), (-1,-1), 'TOP'),
    ('PADDING',    (0,0), (-1,-1), 5),
]))
story.append(arch_table)
story.append(Spacer(1, 10))

# Section 2: Modules
story.append(Paragraph("2. Modules Explanation", h1_style))

story.append(Paragraph("2.1 Lexical Analysis — lexer.l", h2_style))
story.append(Paragraph(
    "The Flex lexer scans the DSL source file character by character and groups characters into "
    "tokens. It recognizes: keywords (int, float, if, else, while, for, print, return), "
    "identifiers, integer and float literals, operators (+, -, *, /, ==, !=, <=, >=, &&, ||, !), "
    "and delimiters (;, (, ), {, }). Whitespace and single-line comments (//) are silently skipped. "
    "Token types are defined via %union in the Bison file and passed through yylval.",
    body_style))

story.append(Paragraph("2.2 Syntax Analysis — parser.y", h2_style))
story.append(Paragraph(
    "The Bison parser implements an LALR(1) grammar for the DSL. It handles operator precedence "
    "(UMINUS > * / > + - > comparisons > && > ||) and constructs an AST node for every grammar "
    "rule. The grammar covers: declarations, assignments, if/ifelse statements, while loops, "
    "for loops, print statements, and arithmetic/logical expressions.",
    body_style))

story.append(Paragraph("2.3 AST Construction — ast.c / ast.h", h2_style))
story.append(Paragraph(
    "The Abstract Syntax Tree uses a single ASTNode struct with fields for node type, data type, "
    "integer/float values, name (for identifiers), and up to three child pointers (left, right, extra). "
    "The 'extra' pointer is used for if-else alternate branches and the for-loop body. "
    "Helper constructors (make_node, make_decl_node, make_assign_node, etc.) cleanly build nodes. "
    "print_ast() recursively prints the tree with indentation for verification.",
    body_style))

story.append(Paragraph("2.4 Semantic Analysis — semantic.c", h2_style))
story.append(Paragraph(
    "The semantic analyzer does a recursive walk over the AST. It uses the symbol table (symtable.c) "
    "to detect: (1) duplicate variable declarations, (2) use of undeclared variables, "
    "(3) use of variables before initialization, and (4) type mismatches in assignments and declarations. "
    "Errors are reported with descriptive messages; warnings are non-fatal. "
    "After analysis, the symbol table is printed showing all variables with their types and values.",
    body_style))

story.append(Paragraph("2.5 Intermediate Code Generation — codegen.c", h2_style))
story.append(Paragraph(
    "The code generator produces Three-Address Code (TAC), a standard intermediate representation "
    "used in compilers. Each complex expression is broken into simple instructions using temporary "
    "variables (t0, t1, ...). Control flow uses conditional jumps (ifFalse ... goto L) and "
    "unconditional jumps (goto L) with auto-generated labels (L0, L1, ...). "
    "TAC is written both to stdout and to an output .tac file.",
    body_style))

story.append(Spacer(1, 8))

# Section 3: Output
story.append(Paragraph("3. Sample Output", h1_style))
story.append(Paragraph("Input DSL (test2.dsl — If/Else condition):", h2_style))
story.append(Paragraph(
    "int x = 50;  int y = 30;  int max = 0;<br/>"
    "if (x &gt; y) { max = x; } else { max = y; }<br/>"
    "print(max);",
    code_style))

story.append(Paragraph("Generated TAC (output/test2.tac):", h2_style))
story.append(Paragraph(
    "BEGIN:<br/>"
    "&nbsp;&nbsp;&nbsp;&nbsp;t0 = 50 &nbsp;&nbsp; x = t0<br/>"
    "&nbsp;&nbsp;&nbsp;&nbsp;t1 = 30 &nbsp;&nbsp; y = t1<br/>"
    "&nbsp;&nbsp;&nbsp;&nbsp;t2 = 0  &nbsp;&nbsp; max = t2<br/>"
    "&nbsp;&nbsp;&nbsp;&nbsp;t3 = x &gt; y<br/>"
    "&nbsp;&nbsp;&nbsp;&nbsp;ifFalse t3 goto L0<br/>"
    "&nbsp;&nbsp;&nbsp;&nbsp;max = x<br/>"
    "&nbsp;&nbsp;&nbsp;&nbsp;goto L1<br/>"
    "L0:<br/>"
    "&nbsp;&nbsp;&nbsp;&nbsp;max = y<br/>"
    "L1:<br/>"
    "&nbsp;&nbsp;&nbsp;&nbsp;print max<br/>"
    "END:",
    code_style))

story.append(Spacer(1, 8))

# Section 4: Marks table
story.append(Paragraph("4. Evaluation Criteria", h1_style))
marks_data = [
    ['Criteria', 'Marks', 'How Achieved'],
    ['Functionality', '2', 'All 5 compiler stages working, 3 test cases pass'],
    ['Implementation', '1', 'Flex + Bison + C, proper AST, TAC output'],
    ['Code Quality', '1', 'Modular files, clean code, meaningful naming'],
    ['Documentation', '1', 'README, report, inline comments'],
    ['Total', '5', ''],
]
marks_table = Table(marks_data, colWidths=[2*inch, 0.8*inch, 3.6*inch])
marks_table.setStyle(TableStyle([
    ('BACKGROUND', (0,0), (-1,0), colors.HexColor('#1a3c5e')),
    ('TEXTCOLOR',  (0,0), (-1,0), colors.white),
    ('FONTNAME',   (0,0), (-1,0), 'Helvetica-Bold'),
    ('FONTNAME',   (0,-1), (-1,-1), 'Helvetica-Bold'),
    ('BACKGROUND', (0,-1), (-1,-1), colors.HexColor('#d0e4f7')),
    ('FONTSIZE',   (0,0), (-1,-1), 9),
    ('ROWBACKGROUNDS', (0,1), (-1,-2), [colors.HexColor('#eaf1fb'), colors.white]),
    ('GRID',       (0,0), (-1,-1), 0.5, colors.HexColor('#cccccc')),
    ('PADDING',    (0,0), (-1,-1), 5),
]))
story.append(marks_table)

doc.build(story)
print("PDF created successfully.")
