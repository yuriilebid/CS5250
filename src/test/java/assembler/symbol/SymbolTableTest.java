package assembler.symbol;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class SymbolTableTest {

    @Test
    void hasPredefinedSymbols() {
        SymbolTable t = new SymbolTable();
        assertEquals(0, t.getAddress("SP"));
        assertEquals(1, t.getAddress("LCL"));
        assertEquals(2, t.getAddress("ARG"));
        assertEquals(3, t.getAddress("THIS"));
        assertEquals(4, t.getAddress("THAT"));
        assertEquals(0, t.getAddress("R0"));
        assertEquals(15, t.getAddress("R15"));
        assertEquals(16384, t.getAddress("SCREEN"));
        assertEquals(24576, t.getAddress("KBD"));
    }

    @Test
    void allocatesVariablesFrom16() {
        SymbolTable t = new SymbolTable();
        int a = t.resolveOrAllocateVariable("foo");
        int b = t.resolveOrAllocateVariable("bar");
        int a2 = t.resolveOrAllocateVariable("foo");
        assertEquals(16, a);
        assertEquals(17, b);
        assertEquals(a, a2);
    }
}

