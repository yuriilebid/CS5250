package assembler.code;

import java.util.HashMap;
import java.util.Map;

public final class CompTable {
    private static final Map<String, String> M = new HashMap<>();

    static {
        M.put("0", "0101010");
        M.put("1", "0111111");
        M.put("-1", "0111010");
        M.put("D", "0001100");
        M.put("A", "0110000");
        M.put("!D", "0001101");
        M.put("!A", "0110001");
        M.put("-D", "0001111");
        M.put("-A", "0110011");
        M.put("D+1", "0011111");
        M.put("A+1", "0110111");
        M.put("D-1", "0001110");
        M.put("A-1", "0110010");
        M.put("D+A", "0000010");
        M.put("D-A", "0010011");
        M.put("A-D", "0000111");
        M.put("D&A", "0000000");
        M.put("D|A", "0010101");
        M.put("M", "1110000");
        M.put("!M", "1110001");
        M.put("-M", "1110011");
        M.put("M+1", "1110111");
        M.put("M-1", "1110010");
        M.put("D+M", "1000010");
        M.put("D-M", "1010011");
        M.put("M-D", "1000111");
        M.put("D&M", "1000000");
        M.put("D|M", "1010101");
    }

    private CompTable() {
    }

    public static String bits(String c) {
        String b = M.get(c);
        if (b == null) throw new IllegalArgumentException("bad comp: " + c);
        return b;
    }
}

