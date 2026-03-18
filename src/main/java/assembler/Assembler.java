package assembler;

import assembler.io.FileService;
import assembler.model.Instruction;
import assembler.parser.CommandType;
import assembler.parser.Parser;
import assembler.symbol.SymbolTable;
import assembler.translator.InstructionTranslator;

import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

public class Assembler {

    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("Usage: java assembler.Assembler <Prog.asm>");
            System.exit(1);
        }
        Path in = Path.of(args[0]);
        Path out = FileService.defaultOutputPath(in);
        try {
            assemble(in, out);
        } catch (IOException e) {
            System.err.println("I/O error: " + e.getMessage());
            System.exit(2);
        } catch (IllegalArgumentException e) {
            System.err.println("Assembly error: " + e.getMessage());
            System.exit(3);
        }
    }

    public static void assemble(Path in, Path out) throws IOException {
        List<String> lines = FileService.readAllLines(in);
        SymbolTable st = new SymbolTable();
        Parser p = new Parser(lines);
        int rom = 0;
        while (p.hasMoreCommands()) {
            p.advance();
            CommandType t = p.commandType();
            if (t == CommandType.L_COMMAND) {
                String s = p.symbol();
                if (st.contains(s)) {
                    throw new IllegalArgumentException("Label redefined: " + s);
                }
                st.addEntry(s, rom);
            } else if (t == CommandType.A_COMMAND || t == CommandType.C_COMMAND) {
                rom++;
            }
        }
        p.reset();
        InstructionTranslator tr = new InstructionTranslator(st);
        List<String> outLines = new ArrayList<>();
        while (p.hasMoreCommands()) {
            p.advance();
            CommandType t = p.commandType();
            if (t == CommandType.L_COMMAND) continue;
            Instruction ins = p.toInstruction();
            String code = tr.translate(ins);
            outLines.add(code);
        }
        FileService.writeAllLines(out, outLines);
    }
}

