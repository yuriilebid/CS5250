## VMtranslator (C++) — Nand2Tetris Chapters 7 & 8

This folder contains a C++ rewrite of your Python VM translator.

### Build

From `project7-8/translators/cpp/`:

```bash
make
```

This produces `VMtranslator-cpp/VMtranslator`.

### Run (generate `.asm`)

Translate a single file:

```bash
./VMtranslator --no-bootstrap ../../projects/07/StackArithmetic/SimpleAdd/SimpleAdd.vm
```

Translate a directory (all `.vm` inside):

```bash
./VMtranslator --bootstrap ../../projects/08/FunctionCalls/FibonacciElement
```

For a directory input, output is written to `<dir>/<dirName>.asm` (e.g. `projects/08/FunctionCalls/FibonacciElement/FibonacciElement.asm`).

### Taking the required screenshot

You have two kinds of supplied test scripts:

- `*VME.tst`: meant to be run in the **VM Emulator** directly on `.vm`
- `*.tst`: meant to be run in the **CPU Emulator** on the generated `.asm`

If your assignment says “simulation running the supplied `.tst` file”, that usually means:

1. Run the translator to generate the `.asm`
2. Open **CPUEmulator** → **Load Script** → choose the supplied `.tst`
3. Click **Run** (or **Run/Stop**) and take a screenshot of the running test

### Which flag to use (matches the book projects)

- **Project 7 tests** (SimpleAdd, StackTest, BasicTest, PointerTest, StaticTest): use `--no-bootstrap`
- **Project 8 flow tests** (BasicLoop, FibonacciSeries): use `--no-bootstrap`
- **Project 8 function-call tests**:
  - SimpleFunction: usually `--no-bootstrap` (the script sets state)
  - FibonacciElement, StaticsTest: use `--bootstrap` (bootstrap must call `Sys.init`)

