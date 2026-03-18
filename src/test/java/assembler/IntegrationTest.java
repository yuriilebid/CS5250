package assembler;

import assembler.io.FileService;
import org.junit.jupiter.api.Test;

import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class IntegrationTest {

    private Path tmp(String name, List<String> xs) throws Exception {
        Path d = Files.createTempDirectory("hack-asm-test");
        Path a = d.resolve(name + ".asm");
        FileService.writeAllLines(a, xs);
        return a;
    }

    private List<String> run(String base, List<String> asm) throws Exception {
        Path a = tmp(base, asm);
        Path h = a.resolveSibling(base + ".hack");
        Assembler.assemble(a, h);
        assertTrue(Files.exists(h));
        return FileService.readAllLines(h);
    }

    @Test
    void assemblesAddProgram() throws Exception {
        List<String> hack = run("Add", List.of(
                "@2",
                "D=A",
                "@3",
                "D=D+A",
                "@0",
                "M=D"
        ));
        assertEquals(List.of(
                "0000000000000010",
                "1110110000010000",
                "0000000000000011",
                "1110000010010000",
                "0000000000000000",
                "1110001100001000"
        ), hack);
    }

    @Test
    void assemblesMaxWithLabelsAndVariables() throws Exception {
        List<String> hack = run("Max", List.of(
                "@R0",
                "D=M",
                "@R1",
                "D=D-M",
                "@OUTPUT_FIRST",
                "D;JGT",
                "@R1",
                "D=M",
                "@OUTPUT_D",
                "0;JMP",
                "(OUTPUT_FIRST)",
                "@R0",
                "D=M",
                "(OUTPUT_D)",
                "@R2",
                "M=D",
                "(INFINITE_LOOP)",
                "@INFINITE_LOOP",
                "0;JMP"
        ));
        assertEquals("0000000000000000", hack.get(0));
        assertEquals("1111110000010000", hack.get(1));
        assertEquals("0000000000000001", hack.get(2));
    }
}

