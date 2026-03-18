### Hack Assembler (Nand2Tetris Project 6)

This project implements a two-pass Hack assembler in Java for Nand2Tetris Project 6.  
It translates valid Hack assembly (`.asm`) files into Hack binary machine code (`.hack`) that matches the official assembler.

### Features

- **Instruction support**: A-instructions, C-instructions, and label (L) declarations
- **Two-pass assembly**:
  - Pass 1: collect label symbols and ROM addresses
  - Pass 2: translate all A/C instructions, resolving symbols and variables
- **Symbol handling**:
  - Predefined symbols: `SP`, `LCL`, `ARG`, `THIS`, `THAT`, `R0`–`R15`, `SCREEN`, `KBD`
  - Variables allocated starting at RAM address 16
- **Parsing**:
  - Ignores blank lines and `//` comments
  - Supports `dest=comp`, `comp;jump`, `dest=comp;jump`, and `comp` forms

### Project structure

- `assembler.Assembler` — main orchestration class (CLI entry point)
- `assembler.parser.Parser` — parses cleaned assembly lines and exposes `commandType()`, `symbol()`, `dest()`, `comp()`, `jump()`
- `assembler.model.*` — small instruction model (`AInstruction`, `CInstruction`, `LInstruction`)
- `assembler.symbol.SymbolTable` — holds predefined symbols, labels, and variables
- `assembler.code.*` — maps `comp`, `dest`, `jump` mnemonics to bit patterns
- `assembler.translator.InstructionTranslator` — **Strategy-style** translator that turns `Instruction` objects into final 16‑bit binary
- `assembler.io.FileService` — small utility for file I/O and `.asm` → `.hack` filename mapping

### Build

Requires Java 17+ and Maven.

```bash
mvn clean package
```

This produces `target/hack-assembler-1.0-SNAPSHOT.jar`.

### Run

From the project root:

```bash
mvn -q -DskipTests package
java -cp target/hack-assembler-1.0-SNAPSHOT.jar assembler.Assembler Prog.asm
```

This generates `Prog.hack` next to `Prog.asm`.

Example:

```bash
java -cp target/hack-assembler-1.0-SNAPSHOT.jar assembler.Assembler Add.asm
```

You can also run the `Assembler` directly from your IDE using the same single argument: the path to the `.asm` file.

### Tests

Run all tests:

```bash
mvn test
```

Included tests:

- **Parser tests**: whitespace/comments handling, A/C/L command parsing
- **Symbol table tests**: predefined symbols, variable allocation from RAM[16]
- **Code table tests**: selected `comp`, `dest`, `jump` encodings
- **Instruction translation tests**: numeric/symbolic A-instructions, C-instruction translation
- **Integration test**: assembles representative `Add` and `Max`-style programs end-to-end and checks resulting machine code

### Design notes & patterns

- **Simple Factory**: `Parser.toInstruction()` creates `AInstruction`, `CInstruction`, or `LInstruction` objects from the current command.
- **Strategy-style translator**: `InstructionTranslator` encapsulates the translation strategy for instruction objects, delegating to `CompTable`, `DestTable`, and `JumpTable` for mnemonic→bit lookups.
- No Mediator or Abstract Factory are used; they would not improve clarity for this small assembler and would add unnecessary layers.

