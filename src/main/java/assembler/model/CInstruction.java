package assembler.model;

public class CInstruction implements Instruction {
    private final String dest;
    private final String comp;
    private final String jump;

    public CInstruction(String dest, String comp, String jump) {
        this.dest = dest;
        this.comp = comp;
        this.jump = jump;
    }

    public String getDest() {
        return dest;
    }

    public String getComp() {
        return comp;
    }

    public String getJump() {
        return jump;
    }
}

