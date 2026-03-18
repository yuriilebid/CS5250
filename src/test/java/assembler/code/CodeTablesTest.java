package assembler.code;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

class CodeTablesTest {

    @Test
    void compDestJumpEncodingsMatchSpec() {
        assertEquals("0101010", CompTable.bits("0"));
        assertEquals("0110000", CompTable.bits("A"));
        assertEquals("1110000", CompTable.bits("M"));

        assertEquals("000", DestTable.bits(null));
        assertEquals("001", DestTable.bits("M"));
        assertEquals("111", DestTable.bits("AMD"));

        assertEquals("000", JumpTable.bits(null));
        assertEquals("001", JumpTable.bits("JGT"));
        assertEquals("111", JumpTable.bits("JMP"));
    }
}

