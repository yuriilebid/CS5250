package assembler.parser;

import assembler.model.AInstruction;
import assembler.model.CInstruction;
import assembler.model.Instruction;
import assembler.model.LInstruction;

import java.util.ArrayList;
import java.util.List;

public class Parser {

    private final List<String> cmds;
    private int i = -1;
    private String cur;

    public Parser(List<String> lines) {
        this.cmds = new ArrayList<>();
        for (String s : lines) {
            String c = strip(s);
            if (!c.isEmpty()) cmds.add(c);
        }
    }

    private String strip(String s) {
        int k = s.indexOf("//");
        if (k != -1) s = s.substring(0, k);
        return s.trim();
    }

    public boolean hasMoreCommands() {
        return i + 1 < cmds.size();
    }

    public void advance() {
        if (!hasMoreCommands()) throw new IllegalStateException("no more");
        i++;
        cur = cmds.get(i);
    }

    public void reset() {
        i = -1;
        cur = null;
    }

    public CommandType commandType() {
        if (cur == null) return CommandType.NONE;
        if (cur.startsWith("@")) return CommandType.A_COMMAND;
        if (cur.startsWith("(") && cur.endsWith(")")) return CommandType.L_COMMAND;
        return CommandType.C_COMMAND;
    }

    public String symbol() {
        CommandType t = commandType();
        if (t == CommandType.A_COMMAND) return cur.substring(1);
        if (t == CommandType.L_COMMAND) return cur.substring(1, cur.length() - 1);
        throw new IllegalStateException("bad symbol()");
    }

    public String dest() {
        if (commandType() != CommandType.C_COMMAND) return null;
        int eq = cur.indexOf('=');
        if (eq == -1) return null;
        return cur.substring(0, eq);
    }

    public String comp() {
        if (commandType() != CommandType.C_COMMAND) return null;
        int eq = cur.indexOf('=');
        int s = cur.indexOf(';');
        int a = (eq == -1) ? 0 : eq + 1;
        int b = (s == -1) ? cur.length() : s;
        return cur.substring(a, b);
    }

    public String jump() {
        if (commandType() != CommandType.C_COMMAND) return null;
        int s = cur.indexOf(';');
        if (s == -1) return null;
        return cur.substring(s + 1);
    }

    public Instruction toInstruction() {
        CommandType t = commandType();
        return switch (t) {
            case A_COMMAND -> new AInstruction(symbol());
            case C_COMMAND -> new CInstruction(dest(), comp(), jump());
            case L_COMMAND -> new LInstruction(symbol());
            default -> throw new IllegalStateException("no cmd");
        };
    }
}

