package assembler.code;

import java.util.HashMap;
import java.util.Map;

public final class JumpTable {
    private static final Map<String, String> J = new HashMap<>();

    static {
        J.put(null, "000");
        J.put("", "000");
        J.put("JGT", "001");
        J.put("JEQ", "010");
        J.put("JGE", "011");
        J.put("JLT", "100");
        J.put("JNE", "101");
        J.put("JLE", "110");
        J.put("JMP", "111");
    }

    private JumpTable() {
    }

    public static String bits(String j) {
        String b = J.get(j);
        if (b == null) throw new IllegalArgumentException("bad jump: " + j);
        return b;
    }
}

