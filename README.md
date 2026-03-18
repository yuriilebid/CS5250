This project implements a two-pass Hack assembler in Java for Nand2Tetris Project 6.  
It translates valid Hack assembly (`.asm`) files into Hack binary machine code (`.hack`) that matches the official assembler.

Requires Java 17+ and Maven.

mvn clean package
mvn -q -DskipTests package
java -cp target/hack-assembler-1.0-SNAPSHOT.jar assembler.Assembler Prog.asm
