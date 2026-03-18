package assembler.symbol;

import java.util.HashMap;
import java.util.Map;

public class SymbolTable {

    private final Map<String, Integer> t = new HashMap<>();
    private int next = 16;

    public SymbolTable() {
        t.put("SP", 0);
        t.put("LCL", 1);
        t.put("ARG", 2);
        t.put("THIS", 3);
        t.put("THAT", 4);
        for (int i = 0; i <= 15; i++) {
            t.put("R" + i, i);
        }
        t.put("SCREEN", 16384);
        t.put("KBD", 24576);
    }

    public void addEntry(String s, int a) {
        t.put(s, a);
    }

    public boolean contains(String s) {
        return t.containsKey(s);
    }

    public int getAddress(String s) {
        Integer x = t.get(s);
        if (x == null) throw new IllegalArgumentException("Unknown symbol: " + s);
        return x;
    }

    public int resolveOrAllocateVariable(String s) {
        if (!t.containsKey(s)) t.put(s, next++);
        return t.get(s);
    }
}

