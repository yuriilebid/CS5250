package assembler.code;

import java.util.HashMap;
import java.util.Map;

public final class DestTable {
    private static final Map<String, String> D = new HashMap<>();

    static {
        D.put(null, "000");
        D.put("", "000");
        D.put("M", "001");
        D.put("D", "010");
        D.put("MD", "011");
        D.put("A", "100");
        D.put("AM", "101");
        D.put("AD", "110");
        D.put("AMD", "111");
    }

    private DestTable() {
    }

    public static String bits(String d) {
        String b = D.get(d);
        if (b == null) throw new IllegalArgumentException("bad dest: " + d);
        return b;
    }
}

