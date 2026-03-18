package assembler.parser;

import assembler.model.AInstruction;
import assembler.model.CInstruction;
import assembler.model.LInstruction;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class ParserTest {

    @Test
    void parsesACommand() {
        Parser p = new Parser(List.of("@21"));
        assertTrue(p.hasMoreCommands());
        p.advance();
        assertEquals(CommandType.A_COMMAND, p.commandType());
        assertEquals("21", p.symbol());
        assertTrue(p.toInstruction() instanceof AInstruction);
    }

    @Test
    void parsesCCommandVariants() {
        Parser p = new Parser(List.of(
                "D=M",
                "D;JGT",
                "0;JMP",
                "M+1"
        ));
        p.advance();
        assertEquals(CommandType.C_COMMAND, p.commandType());
        assertEquals("D", p.dest());
        assertEquals("M", p.comp());
        assertNull(p.jump());
        assertTrue(p.toInstruction() instanceof CInstruction);

        p.advance();
        assertNull(p.dest());
        assertEquals("D", p.comp());
        assertEquals("JGT", p.jump());

        p.advance();
        assertEquals("0", p.comp());
        assertEquals("JMP", p.jump());

        p.advance();
        assertNull(p.dest());
        assertEquals("M+1", p.comp());
        assertNull(p.jump());
    }

    @Test
    void parsesLabelCommand() {
        Parser p = new Parser(List.of("(LOOP)"));
        p.advance();
        assertEquals(CommandType.L_COMMAND, p.commandType());
        assertEquals("LOOP", p.symbol());
        assertTrue(p.toInstruction() instanceof LInstruction);
    }

    @Test
    void ignoresWhitespaceAndComments() {
        Parser p = new Parser(List.of(
                "   // comment only",
                "",
                "   @2   // with comment",
                "D=A",
                "   (LOOP)   "
        ));
        assertTrue(p.hasMoreCommands());
        p.advance();
        assertEquals(CommandType.A_COMMAND, p.commandType());
        assertEquals("2", p.symbol());

        p.advance();
        assertEquals(CommandType.C_COMMAND, p.commandType());
        assertEquals("D", p.dest());
        assertEquals("A", p.comp());

        p.advance();
        assertEquals(CommandType.L_COMMAND, p.commandType());
        assertEquals("LOOP", p.symbol());
        assertFalse(p.hasMoreCommands());
    }
}

