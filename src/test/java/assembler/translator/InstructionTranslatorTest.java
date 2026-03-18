package assembler.translator;

import assembler.model.AInstruction;
import assembler.model.CInstruction;
import assembler.symbol.SymbolTable;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

class InstructionTranslatorTest {

    @Test
    void translatesNumericAInstruction() {
        SymbolTable t = new SymbolTable();
        InstructionTranslator tr = new InstructionTranslator(t);
        String b = tr.translate(new AInstruction("21"));
        assertEquals("0000000000010101", b);
    }

    @Test
    void translatesSymbolicAInstructionVariable() {
        SymbolTable t = new SymbolTable();
        InstructionTranslator tr = new InstructionTranslator(t);
        String x = tr.translate(new AInstruction("foo"));
        String y = tr.translate(new AInstruction("foo"));
        assertEquals(x, y);
        assertEquals("0000000000010000", x);
    }

    @Test
    void translatesCInstruction() {
        SymbolTable t = new SymbolTable();
        InstructionTranslator tr = new InstructionTranslator(t);
        String c = tr.translate(new CInstruction("D", "M+1", null));
        assertEquals("1111110111010000", c);
    }
}

