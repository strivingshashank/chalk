
# Chalk - a math scripting language

> An ongoing deep dive into compiler construction: lexing, parsing, AST design, evaluation, and everything that comes after.

Chalk is a scripting language made for math guys (definitely not me).

A handwritten interpreter, built from scratch in C - no parser generators, no libraries.
It currently supports arithmetic expressions, variable bindings, and print statements - with a lot more planned.

Source files use the `.chalk` extension.

The language is intentionally minimal for now. The goal isn't a production language - it's a clear, readable, hand-authored implementation of every stage in a compiler, grown incrementally as each concept is understood and implemented from scratch.


<hr>

## Current Syntax
What Chalk can parse and evaluate today.
#### Print Statement
Evaluates an expression and prints the result.

`print: <expression>`

```
print: 67;
print: 4 ^ 7 ^ 3;
print: (1 + 2) *3
```
#### Let Statement
Name an expression.

> ⚠️ `let` is parsed and added to the AST but not yet evaluated - symbol table is in progress.

`let <name>: <expression>;`

```
let x: 10;
let y: 20;
let z: x + y;
print: z - x + y;
```

#### Expressions

| Operator | Description              | Associativity  |
| -------- | ------------------------ | -------------- |
| `+` `-`  | Addition, Subtraction    | Left           |
| `*` `/`  | Multiplication, Division | Left           |
| `^`      | Exponentiation           | Right          |
| `-`      | Unary negation           | Right (prefix) |
| `(` `)`  | Grouping                 | NIL            |

Precedence (lowest -> highest): +|- -> /|* -> ^ -> unary - -> ()

#### Example

```
let a: 10;
let b: 3;
print: a + b * 2
print: 2 ^ 3 ^ 2
print: (a + b) * -1
```

## Architecture

#### Grammar

```
PROGRAM     ->  (STATEMENT)* EOF
STATEMENT   ->  PRINT_STMT | LET_STMT ";"
PRINT_STMT  ->  "print" ":" EXPRESSION
LET_STMT    ->  "let" IDENT ":" EXPRESSION
EXPRESSION  ->  TERM (("+"|"-") TERM)*
TERM        ->  POWER (("*"|"/") POWER)*
POWER       ->  UNARY ("^" POWER)?
UNARY       ->  "-" UNARY | PRIMARY
PRIMARY     ->  NUMBER | IDENT | "(" EXPRESSION ")"
```

#### Pipeline

```
 ┌─────────────┐
 │ .chalk file │
 └──────┬──────┘
        │  fread()
        ▼
 ┌─────────────┐
 │   Source    │  char* in memory
 │   String    │
 └──────┬──────┘
        │  lexer_new()
        ▼
 ┌─────────────┐
 │    Lexer    │  Tokenises the source on demand (lazy)
 │  (lexer.c)  │  Supports peek (1-token lookahead)
 └──────┬──────┘
        │  lexer_next() / lexer_peek()
        ▼
 ┌─────────────┐
 │   Parser    │  Recursive descent
 │  (parser.c) │  Consumes tokens, builds AST
 └──────┬──────┘
        │  parse_program()
        ▼
 ┌─────────────┐
 │     AST     │  Binary tree of ast_node_t
 │             │  PROGRAM nodes chain via right pointer
 └──────┬──────┘
        │  ast_eval()          ← work in progress
        ▼
 ┌─────────────┐
 │   Output    │  Evaluated results written to stream
 └─────────────┘
```

#### Example: AST Dump

Input: `let x: 2 ^ 3 + 1;`

```
PROGRAM
└── LET
    ├── IDENT(x)
    └── ADD
        ├── POW
        │   ├── NUM(2)
        │   └── NUM(3)
        └── NUM(1)
```

#### File Structure

```
chalk/
├── include/
│   ├── common.h       - die(), mem_alloc(), mem_free()
│   ├── lexer.h        - token types, lexer API
│   └── parser.h       - AST node types, parse/eval/dump API
├── src/
│   ├── common.c       - utility implementations
│   ├── lexer.c        - tokeniser, keyword detection, peek support
│   ├── parser.c       - recursive descent parser, AST evaluator, AST dump
│   └── main.c         - entry point, file loading
├── example/
│   └── ex3.chalk      - current test program
└── tests/
    ├── output_lexer.txt
    ├── output_parser.txt
    └── output_program.txt
```

## Building

```bash
# Clone and build
git clone https://github.com/<you>/chalk
cd chalk
make

# Run
make run

# Run tests
make test

# Generate and open documentation
make docs
```

## Progress

| Stage                | Status         | Notes                                               |
| -------------------- | -------------- | --------------------------------------------------- |
| Lexer                | ✅ Done         | Numbers, identifiers, all operators, keywords       |
| Lexer peek           | ✅ Done         | 1-token lookahead via cached peeked token           |
| Lexer dump           | ✅ Done         | Full token stream to any `FILE*`                    |
| Parser               | ✅ Done         | Full recursive descent matching the grammar         |
| AST construction     | ✅ Done         | Binary `ast_node_t` tree, PROGRAM chain via `right` |
| AST dump             | ✅ Done         | Indented tree to any `FILE*`                        |
| Evaluation - `print` | ✅ Done         | Evaluates and outputs expression results            |
| Evaluation - `let`   | 🔧 In progress | Parsed and in AST; blocked on symbol table          |
| Symbol table         | 🔧 In progress | Core blocker for variable evaluation                |
| Error reporting      | ⚠️ Minimal     | `die()` only; no line/column info yet               |

## What's Next

This is where most of the interesting work is. Chalk is being built stage by stage - the front-end is mostly done, and the focus is now shifting toward evaluation, semantics, and eventually compilation.

#### Immediate - Unblocking `let`
- [ ] Symbol table - hash map for variable name → value bindings
- [ ] Evaluate `IDENT` nodes via symbol table lookup
- [ ] Evaluate `let` statements end-to-end
- [ ] CLI argument for input file (`./chalk source.chalk`)

#### Near Term - Making it a Real Language
- [ ] Line and column tracking in the lexer for meaningful error messages
- [ ] Re-assignment (`let x: 5; let x: x + 1;`)
- [ ] Boolean expressions (`==`, `!=`, `<`, `>`)
- [ ] `if`/`else` statements
- [ ] While loops

#### Medium Term - Functions and Types
- [ ] User-defined functions (`fn name(args): body`)
- [ ] Basic type system - integers, floats, strings as distinct types
- [ ] Type checking in the evaluator

#### Long Term - Compilation
- [ ] Byte-code design - instruction set for a simple stack machine
- [ ] Byte-code compiler - walk the AST, emit instructions
- [ ] Stack-based virtual machine - execute byte-code directly
- [ ] Standard library - string ops, basic I/O, math

### Stretch
- [ ] A REPL (`chalk` with no arguments drops into an interactive shell)
- [ ] Self-hosting - a subset of Chalk written in Chalk

---
