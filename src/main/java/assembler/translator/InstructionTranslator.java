package assembler.translator;

import assembler.code.CompTable;
import assembler.code.DestTable;
import assembler.code.JumpTable;
import assembler.model.AInstruction;
import assembler.model.CInstruction;
import assembler.model.Instruction;
import assembler.symbol.SymbolTable;

public class InstructionTranslator {

    private final SymbolTable st;

    public InstructionTranslator(SymbolTable st) {
        this.st = st;
    }

    public String translate(Instruction ins) {
        if (ins instanceof AInstruction a) return a(a);
        if (ins instanceof CInstruction c) return c(c);
        throw new IllegalArgumentException("bad ins " + ins.getClass());
    }

    private String a(AInstruction i) {
        String s = i.getSymbol();
        int v;
        try {
            v = Integer.parseInt(s);
        } catch (NumberFormatException e) {
            if (st.contains(s)) v = st.getAddress(s);
            else v = st.resolveOrAllocateVariable(s);
        }
        if (v < 0 || v > 0x7FFF) throw new IllegalArgumentException("A out of range: " + v);
        String b = Integer.toBinaryString(v);
        String pad = "0".repeat(15 - b.length()) + b;
        return "0" + pad;
    }

    private String c(CInstruction i) {
        String cb = CompTable.bits(i.getComp());
        String db = DestTable.bits(i.getDest());
        String jb = JumpTable.bits(i.getJump());
        return "111" + cb + db + jb;
    }
}

